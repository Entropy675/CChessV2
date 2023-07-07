#ifndef CHESSENGINE
#define CHESSENGINE

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <thread>
#include <mutex>

#include <vector>
#include "Board.h"
#include "defs.h"

class ChessEngine
{
	public:
	ChessEngine();
	~ChessEngine();
	
	// starts the chess engine server, socket defined in defs.h
	int startServer();
	
	
	private:
	void handleClient(int clientSocket);
	
	std::mutex accessResourceAsync; // lock this mutex when accessing resources in handleClient
	// you can aquire the mutex via std::lock_guard<std::mutex> lock(accessResourceAsync); which is RAII (Resource Acquisition Is Initialization) wrapper
    std::vector<std::thread> clientThreads;
	
    int serverSocket;
    sockaddr_in serverAddress{};
	
	//Board game;
};



#endif