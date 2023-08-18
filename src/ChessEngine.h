#ifndef CHESSENGINE
#define CHESSENGINE

#include <iostream>
#include <cstring>

#ifdef _WIN32
	#define OP	1
#elif defined(__linux__)
	#define OP	2
#else
    #error "ChessEngine.h: Unsupported platform - compilation stopped."
#endif

#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

#include "SocketHandler.h"
#include "Board.h"
#include "defs.h"


class ChessEngine
{
	public:
	ChessEngine();
	~ChessEngine();
	
	// starts the chess engine server, port defined in defs.h -> delegates to SocketHandler interface for the OS
	int startServer();
	
	private:
	SocketHandler* SocketCtrl = nullptr;
	
	Board game;
};



#endif