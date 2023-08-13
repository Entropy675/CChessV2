#include "ChessEngine.h"

#ifdef _WIN32
	#include "WindowsSocket.h"
#elif defined(__linux__)
	#include "LinuxSocket.h"
#else
    #error "ChessEngine.cc: Unsupported platform - compilation stopped."
#endif

ChessEngine::ChessEngine()
{
	#ifdef _WIN32
		SocketCtrl = new WindowsSocket();
	#elif defined(__linux__)
		SocketCtrl = new LinuxSocket();
	#else
		#error "ChessEngine.cc: Unsupported platform - compilation stopped."
	#endif
	
}

ChessEngine::~ChessEngine()
{
	if(SocketCtrl != nullptr)
		delete SocketCtrl;
}


int ChessEngine::startServer()
{
    return SocketCtrl->startServer();
}
