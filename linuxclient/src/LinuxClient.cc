#include "LinuxClient.h"

int LinuxClient::startConnection()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (clientSocket < 0) 
	{
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
	
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) 
	{
        std::cerr << "Invalid address." << std::endl;
        return 1;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) 
	{
        std::cerr << "Connection failed." << std::endl;
        return 1;
    }
	
	std::cout << "Connected to the chess engine." << std::endl;

    // Exchange data with the chess engine through 'clientSocket'

	
	while(true)
	{
		
		
		
		
		
	}


    close(clientSocket);

    return 0;
}
