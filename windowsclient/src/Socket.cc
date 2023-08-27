#include "Socket.h"

Socket::Socket(const char* ip)
	: connectIP(ip) 
{	
	// initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Socket::Socket(): Failed to initialize Winsock." << std::endl;
        throw "Critical Socket Error, Socket could not be initialized.";
    }
}

Socket::~Socket()
{
    closesocket(clientSocket);
	
    // Cleanup Winsock
    WSACleanup();
}

bool Socket::isConnected()
{
	return !disconnected;
}

std::string& Socket::getIP()
{
	return connectIP;
}

void Socket::setIP(const char* inp)
{
	connectIP = inp; 
}

int Socket::startConnection()
{
	if(!disconnected)
	{
		closesocket(clientSocket);
		disconnected = true;
		return 1;
	}
	
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Socket::startConnection(): Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(connectIP.c_str());

    // Connect to server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
	{
        std::cout << "Socket::startConnection(): Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

	disconnected = false;
    std::cout << "Socket::startConnection(): Connected to the server." << std::endl;
	return 0;
}

void Socket::sendData(const char* message)
{
    if (send(clientSocket, disconnected ? "" : message, disconnected ? 0 : strlen(message), 0) == SOCKET_ERROR) 
	{
        std::cout << "Socket::sendData(): Failed to send data. Error code: " << WSAGetLastError() << std::endl;
        // fail code goes here
    }
}

bool Socket::receiveData(std::string& out)
{
    char buffer[RECIEVED_DATA_BUFF]; // Adjust the buffer size as needed
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived == SOCKET_ERROR) {
        return false; // Return early because there is no message
    }

    // Null-terminate the received data
    buffer[bytesReceived] = '\0';

    out = std::string(buffer);
	return true;
}