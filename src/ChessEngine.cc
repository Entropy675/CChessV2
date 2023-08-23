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
	std::cout << "ChessEngine::startServer(): Socket Start Server STATUS: " << status << std::endl;
	
	if(!status)
		responseLoop();
	
	return status;
}

void ChessEngine::responseLoop()
{
	int pingCounter = 0;
	std::string data;
	
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
			else if(pingCounter++ > 200)
			{
				pingCounter = 0;
				std::cout << "Ping... " << std::endl;
			}
		}
		data = "ping... " + std::to_string(pingCounter) + " -> ";
		
		for(int i = 0; i < pingCounter; i++)
			if(i % 4 == 0)
				data += std::to_string(i);
		
		SocketCtrl->sendData(data);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(ENGINE_DELAY_MS));
	}
}