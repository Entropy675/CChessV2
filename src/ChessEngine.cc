#include "ChessEngine.h"

#ifdef _WIN32
	#include "WindowsSocket.h"
#elif defined(__linux__)
	#include "LinuxSocket.h"
#else
    #error "ChessEngine.cc: Unsupported platform - compilation stopped."
#endif

#include <queue>

ChessEngine::ChessEngine()
{
	#ifdef _WIN32
		SocketCtrl = new WindowsSocket();
	#elif defined(__linux__)
		SocketCtrl = new LinuxSocket();
	#else
		#error "ChessEngine::ChessEngine(): Unsupported platform - compilation stopped."
	#endif
	
}

ChessEngine::~ChessEngine()
{
	if(SocketCtrl != nullptr)
		delete SocketCtrl;
}


int ChessEngine::startServer()
{
    int status = SocketCtrl->startServer();
	std::cout << "ChessEngine::startServer(): Going into response queue... MS:" << ENGINE_DELAY_MS << std::endl;
	while(true)
	{
		{
			std::lock_guard<std::mutex> lock(SocketCtrl->queueMutex);
			std::queue<std::string>& cmdQueue = SocketCtrl->accessCommandQueue(lock);
			// CORE LOOP
			if(!cmdQueue.empty())
			{
				std::cout << cmdQueue.front() << std::endl;
				cmdQueue.pop();
			}
			else
			{
				std::cout << "Empty... " << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(ENGINE_DELAY_MS));
	}
}
