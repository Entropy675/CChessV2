#include "SocketHandler.h"


SocketHandler::SocketHandler()
	: serverSocketEnabled(false) {}

SocketHandler::~SocketHandler()
{
	serverSocketEnabled = false;

	try
	{		
		if (acceptThread.joinable())
			acceptThread.join();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Accept thread could not be joined: " << e.what() << std::endl;
	}

	{
		std::lock_guard<std::mutex> lock(recieveThreadsMutex);
		for (auto& pair : recieveThreads) 
		{
			std::thread& t = pair.second;
			try
			{		
				if (t.joinable())
					t.join();
			}
			catch(const std::exception& e)
			{
				std::cerr << "Recieve thread could not be joined: " << e.what() << std::endl;
			}
		}
	}
}

// Returns a reference to the queue. The lockguard passed better be locking the mutex.
std::queue<std::string>& SocketHandler::accessCommandQueue(std::lock_guard<std::mutex>& lockGuard)
{
	// The input requirement of lockGuard is to make sure no one forgets.
    return cmdQueue;
}
