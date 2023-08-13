#include SocketHandler.h


SocketHandler::SocketHandler()
{
	
}

SocketHandler::~SocketHandler()
{
	
}


// Returns a reference to the queue. The lockguard passed better be locking the mutex.
std::queue<std::string>& SocketHandler::accessCommandQueue(std::lock_guard<std::mutex>& lockGuard)
{
    if (!lockGuard.owns_lock()) 
        throw std::runtime_error("SocketHandler::accessCommandQueue(): Uninitialized lock guard passed to accessCommandQueue. Please ensure that it is initialized, and the correct mutex is locked!"); 
    return cmdQueue;
}
