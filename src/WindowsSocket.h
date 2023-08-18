#ifndef WINDOWSSOCKET_H
#define WINDOWSSOCKET_H

#include "SocketHandler.h"	
#include "defs.h"

// Windows
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <chrono>
#include <thread>
#include <set>


class WindowsSocket : public SocketHandler
{
	public:
	WindowsSocket();
	~WindowsSocket();
	
	virtual int startServer() override;
	virtual int sendData(const std::string& data) override;
	virtual int sendData(const std::string& data, int cs) override;

	private:
	virtual void acceptConnections() override; // should be put on a diff thread
	virtual void receiveDataToQueue() override;
	virtual void receiveData(std::string& out, int cs) override;

	void closeDisconnectedSockets(int soc);

    std::thread acceptThread;
    std::thread recieveThread;
	
	WSADATA wsaData;
	std::vector<SOCKET> clientSockets;
	std::mutex clientSocketsMutex; // use std::lock_guard<std::mutex> lock(clientSocketsMutex); object to simplify lock/unlocking (its RAII)
	SOCKET serverSocket;
	bool socketEnabled;
};




#endif