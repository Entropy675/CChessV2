#include "SocketHandler.h"


SocketHandler::SocketHandler()
{
	
}

SocketHandler::~SocketHandler()
{
	
}


// Returns a reference to the queue. The lockguard passed better be locking the mutex.
std::queue<std::string>& SocketHandler::accessCommandQueue(std::lock_guard<std::mutex>& lockGuard)
{
	// could potentially add some checking here. The input requirement of lockGuard is to make sure no one forgets.
    return cmdQueue;
}
