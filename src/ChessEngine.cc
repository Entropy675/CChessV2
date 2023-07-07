#include "ChessEngine.h"

ChessEngine::ChessEngine()
{
	
}

ChessEngine::~ChessEngine()
{
	
}

void ChessEngine::handleClient(int clientSocket) 
{
    // Handle communication with the client here
    // Example: exchange data using 'clientSocket'

    close(clientSocket);
}

int ChessEngine::startServer()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (serverSocket < 0) 
	{
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) 
	{
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }

    listen(serverSocket, MAX_CLIENTS);

    std::cout << "Waiting for clients to connect..." << std::endl;

    while (true) 
	{
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
        if (clientSocket < 0) 
		{
            std::cerr << "Failed to accept connection." << std::endl;
            return 1;
        }

        std::cout << "Client connected." << std::endl;

        // Start a new thread to handle the client
        std::thread thread([this, clientSocket]() 
		{ 
			handleClient(clientSocket); 
		});
        clientThreads.push_back(std::move(thread));
    }

    // Wait for all client threads to finish
    for (auto& thread : clientThreads) 
	{
        thread.join();
    }

    close(serverSocket);

    return 0;
}
