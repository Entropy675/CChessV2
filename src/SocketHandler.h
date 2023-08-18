#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <iostream>
#include <queue>
#include <mutex>
#include <unordered_map>

class SocketHandler
{
public:
    SocketHandler();
    virtual ~SocketHandler();

    virtual int startServer() = 0;
    virtual int sendData(const std::string& data) = 0;
    virtual int sendData(const std::string& data, int cs) = 0;

    // Returns a reference to the queue, takes in a lock guard mutex - YOU MUST INITIALIZE THE LOCKGUARD TO THE MUTEX before passing it in.
    std::queue<std::string>& accessCommandQueue(std::lock_guard<std::mutex>& lockGuard);
    std::mutex queueMutex; // Use std::lock_guard<std::mutex> lock(queueMutex); object to simplify lock/unlocking (RAII).

protected:
    virtual void acceptConnections() = 0;  // Should be put on a different thread, run automatically by startServer and joined appropriately in dtor.
    virtual void receiveDataToQueue() = 0; // Should be on a different thread, automatically queues commands received via receiveData() onto the commandQueue.
    virtual void receiveData(std::string& out, int cs) = 0;

	std::unordered_map<int, int> timeoutMap;
private:
    std::queue<std::string> cmdQueue; // you only get this if you go through the accessComandQueue()
};

#endif
