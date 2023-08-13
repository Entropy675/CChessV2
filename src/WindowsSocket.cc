#include "WindowsSocket.h"

WindowsSocket::WindowsSocket() : socketEnabled(false)
{
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WindowsSocket::WindowsSocket(): WSAStartup failed\n";
		throw "WindowsSocket::WindowsSocket(): Winsock failed to initialize.";
	}
}

WindowsSocket::~WindowsSocket()
{
	socketEnabled = false;
	
	if (acceptThread.joinable())
		acceptThread.join();
	
	if (recieveThread.joinable())
		recieveThread.join();
	
	for(auto& sockets : clientSockets)
		closesocket(sockets);
	
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
			std::cout << "WindowsSocket::SendData(): send failed" << std::endl;
			// This is something to be discussed. See threadssketch.txt.
			// Handle error if needed, or you can loop forever until it succeeds I guess?
			// if it goes on for too long = they dropped connection and you close their socket
			return 1; // Return an error code
		}
		
		return 0;
	} 
	
	for (SOCKET sock : clientSockets) 
	{
		int bytesSent = send(sock, data.c_str(), data.size(), 0);
		if (bytesSent == SOCKET_ERROR) 
		{
			std::cout << "WindowsSocket::SendData(): send failed" << std::endl;
			// This is something to be discussed. See threadssketch.txt.
			// make a list of people that didn't get the message, and keep trying to send it.
			// if it fails too many times = dropped connection
			return 1; // Return an error code
		}
	}
	return 0;
}

void WindowsSocket::receiveData(std::string& out, int cs)
{
	SOCKET clientSocket = static_cast<SOCKET>(cs);

    char buffer[1024]; // Adjust the buffer size as needed
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived == SOCKET_ERROR) {
        return; // Return early because there is no message
    }

    // Null-terminate the received data
    buffer[bytesReceived] = '\0';

    out = std::string(buffer);
}

void WindowsSocket::receiveDataToQueue()
{
	std::string output = "";
	std::queue<std::string> tempQueue;
	
	while(socketEnabled)
	{
		for(auto& socket : clientSockets)
		{
			output = "";
			receiveData(output, static_cast<int>(socket));
			if(output.compare("") != 0)
				tempQueue.push(output); // does not store ref, copies
		}
		
		if(!tempQueue.empty())
		{
			std::lock_guard<std::mutex> lock(SocketHandler::queueMutex);
			std::queue<std::string>& cmdQueue = accessCommandQueue(lock); // if lock has a lock, gives me cmdQueue
			
			while(!tempQueue.empty())
			{
				cmdQueue.push(tempQueue.front());
				tempQueue.pop();
			}
		}
	}
	
	std::cout << "WindowsSocket::receiveDataToQueue(): Exiting... \n";
}
	
void WindowsSocket::acceptConnections()
{
    while (socketEnabled) 
	{
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
            continue;

        // Add the client socket to the vector
        clientSockets.push_back(clientSocket);
    }
	
	std::cout << "WindowsSocket::acceptConnections(): Exiting... \n";
}

int WindowsSocket::startServer()
{
	// Create a socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "WindowsSocket::startServer(): Server socket creation failed\n";
		return 1;
	}

	// Set up sockaddr_in structure	
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cout << "WindowsSocket::startServer(): Server bind failed\n";
		closesocket(serverSocket);
		return 1;
	}
	
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "WindowsSocket::startServer(): Server socket listen failed" << std::endl;
        closesocket(serverSocket);
        return 1;
    }
	
	// Activate threads, passed all fail cases.
	socketEnabled = true;
	
	acceptThread = std::thread(&WindowsSocket::acceptConnections, this);
	recieveThread = std::thread(&WindowsSocket::receiveDataToQueue, this);
	
	return 0;
}
