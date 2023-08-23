#include "WindowsSocket.h"
#include <algorithm>

WindowsSocket::WindowsSocket()
{
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WindowsSocket::WindowsSocket(): WSAStartup failed\n";
		throw "WindowsSocket::WindowsSocket(): Winsock failed to initialize.";
	}
}

WindowsSocket::~WindowsSocket()
{
	{
		std::lock_guard<std::mutex> lock(clientSocketsMutex);
		for(auto& sockets : clientSockets)
			closesocket(sockets);
	}
	
	// Cleanup and close socket
	closesocket(serverSocket);
	WSACleanup();
}

int WindowsSocket::sendData(const std::string& data)
{
	return sendData(data, -1); // Send to all clients using other func and invalid socket
}

int WindowsSocket::sendData(const std::string& data, int cs)
{
	SOCKET clientSocket = static_cast<SOCKET>(cs);

	if (cs != -1) 
	{
		int bytesSent = send(clientSocket, data.c_str(), data.size(), 0);
		if (bytesSent == SOCKET_ERROR) 
		{
			// requires more work
			return 1; // Return an error code
		}
		
		return 0;
	} 
	
	std::lock_guard<std::mutex> lock(clientSocketsMutex);
	for (int& sock : clientSockets) 
	{
		int bytesSent = send(static_cast<SOCKET>(sock), data.c_str(), data.size(), 0);
		if (bytesSent == SOCKET_ERROR) 
		{
			// requires more work
			return 1; // Return an error code
		}
	}
	
	std::this_thread::sleep_for(std::chrono::milliseconds(ENGINE_DELAY_MS));
	return 0;
}

void WindowsSocket::receiveData(std::string& out, int cs)
{
	SOCKET clientSocket = static_cast<SOCKET>(cs);

    char buffer[RECIEVED_DATA_BUFF]; // Adjust the buffer size as needed
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived == SOCKET_ERROR) {
        return; // Return early because there is no message
    }

    // Null-terminate the received data
    buffer[bytesReceived] = '\0';

    out = std::string(buffer);
}

void WindowsSocket::receiveDataToQueue(int socketint)
{
	std::string output = "";
	std::cout << "WindowsSocket::receiveDataToQueue(): Entering... \n";
	
	bool doNotJoin = true;
	
	
	while(serverSocketEnabled && doNotJoin)
	{
		
		receiveData(output, socketint);
		
		if(output.empty())
		{
			if(closeDisconnectedSockets(socketint))
			{
				doNotJoin = false;
			}
		}
		else
		{
			std::cout << "WindowsSocket::receiveDataToQueue(): TEMP recieveThreads size: " << recieveThreads.size() << std::endl;
			std::cout << "WindowsSocket::receiveDataToQueue(): Recieved Data (" << socketint << "): " << output << std::endl;
			
			std::lock_guard<std::mutex> lock(queueMutex);
			std::queue<std::string>& cmdQueue = accessCommandQueue(lock); // if i got a lock, gives me cmdQueue
			std::cout << "WindowsSocket::receiveDataToQueue(): SOC " << socketint << ", cmdQueue.push() " << std::endl;
			cmdQueue.push(output);
		}
	}
	
	std::lock_guard<std::mutex> lock(recieveThreadsMutex);
	//recieveThreads.erase(std::this_thread::get_id()); // removing thread from joinable thread set
	std::cout << "WindowsSocket::receiveDataToQueue(): Exiting... \n";
}
	
bool WindowsSocket::closeDisconnectedSockets(int soc)
{
	std::lock_guard<std::mutex> lock(timeoutMutex);
	int tm = ++timeoutMap[soc];
	if(tm % (SOCKET_TIMEOUT_PING/10) == 0)
		std::cout << "WindowsSocket::closeDisconnectedSockets(): TIME OUT SOC (" << soc << "): " << tm << std::endl;
	if(tm >= SOCKET_TIMEOUT_PING)
	{
		timeoutMap.erase(soc);
		closesocket(static_cast<SOCKET>(soc));
		std::vector<int>::iterator it = std::find(clientSockets.begin(), clientSockets.end(), soc);
		if (it != clientSockets.end())
			clientSockets.erase(it);
		return true;
	}
	
	
	std::this_thread::sleep_for(std::chrono::milliseconds(SOCKET_TIMEOUT_PING/10));
	return false;
}

void WindowsSocket::acceptConnections()
{
	std::cout << "WindowsSocket::acceptConnections(): Entering... \n";
	
    while (serverSocketEnabled) 
	{
        SOCKET clientSocket = accept(static_cast<SOCKET>(serverSocket), NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
            continue;

        // Add the client socket to the vector
		std::cout << "WindowsSocket::acceptConnections(): CLIENT CONNECTED: " <<  static_cast<int>(clientSocket) << std::endl;
        
		{
			std::lock_guard<std::mutex> lock(clientSocketsMutex);
			clientSockets.push_back(static_cast<int>(clientSocket));
		}
		std::thread temp(&WindowsSocket::receiveDataToQueue, this, static_cast<int>(clientSocket));
		{			
			std::lock_guard<std::mutex> lock(recieveThreadsMutex);
			recieveThreads[temp.get_id()] = std::move(temp);
		}
    }
	
	std::cout << "WindowsSocket::acceptConnections(): Exiting... \n";
}

void WindowsSocket::resendOrder(int socket, std::string cmd)
{
	int resetCount = 1;
	bool notSent = true;
	
	while (serverSocketEnabled && resetCount < RESEND_CMD_PING && notSent) 
	{
		resetCount++;
		
		notSent = sendData(cmd, socket);
		
		if(notSent)
			std::this_thread::sleep_for(std::chrono::milliseconds(resetCount*RESEND_CMD_PING));
	}
}

void WindowsSocket::startResendOfOrder(int socket, const std::string& cmd)
{
	std::thread temp(&WindowsSocket::resendOrder, this, socket, cmd);
	temp.detach();
	//resendThreads[temp.get_id()] = std::move(temp);
}

int WindowsSocket::startServer()
{
	// Create a socket
	SOCKET tmp = socket(AF_INET, SOCK_STREAM, 0);
	if (tmp == INVALID_SOCKET) 
	{
		std::cout << "WindowsSocket::startServer(): Server socket creation failed\n";
		return 1;
	}
	
	serverSocket = static_cast<int>(tmp);

	// Set up sockaddr_in structure	
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket
	if (bind(tmp, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
	{
		std::cout << "WindowsSocket::startServer(): Server bind failed\n";
		closesocket(tmp);
		return 1;
	}
	
    if (listen(tmp, SOMAXCONN) == SOCKET_ERROR) 
	{
        std::cout << "WindowsSocket::startServer(): Server socket listen failed" << std::endl;
        closesocket(tmp);
        return 1;
    }
	
	// Activate threads, passed all fail cases.
	serverSocketEnabled = true;
	
	acceptThread = std::thread(&WindowsSocket::acceptConnections, this);
	
	return 0;
}
