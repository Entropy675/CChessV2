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
	std::string command;
	char timeBuffer[100];
	std::time_t currentTime;
	
	while(true)
	{
		command = "\0";
		
		{	
			std::lock_guard<std::mutex> lock(SocketCtrl->queueMutex);
			std::queue<std::string>& cmdQueue = SocketCtrl->accessCommandQueue(lock);
			// CORE LOOP
			if(!cmdQueue.empty())
			{
				command = cmdQueue.front();
				cmdQueue.pop();
			}
		}

		// process command here
		if(!command.empty())
		{
			std::cout << command << std::endl;
		}
		// Get the current time
		currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
		
		if(pingCounter++ > 200)
		{
			pingCounter = 0;
			std::cout << "Ping... " << timeBuffer << std::endl;
			// ping on the server side after 200 iterations...
		}
		

		data = "ping... " + std::to_string(pingCounter) + " -> ";
		
		for(int i = 0; i < 100; i++)
		{
			if(i % 4 == 0)
				data += std::to_string(i);
			if(i % 25 == 0)
				data += "\n";
		}
		// ping on the clients that are connected
		SocketCtrl->sendData(data); // respond all clients connected
		// can also do SocketCtrl->sendData(data, socket); for specific comms.
		
		
		// wait for delay...
		std::this_thread::sleep_for(std::chrono::milliseconds(ENGINE_DELAY_MS));
	}
}