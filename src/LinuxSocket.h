#ifndef LINUXSOCKET_H
#define LINUXSOCKET_H

#include <thread>
#include <iostream>

// LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
	
#include "SocketHandler.h"

class LinuxSocket : public SocketHandler
{
	public:
	LinuxSocket();
	~LinuxSocket();
	
	virtual int startServer() override;
	virtual int sendData(const std::string& data) override;
	virtual int sendData(const std::string& data, int cs) override;
	
	private:
	virtual void acceptConnections() override; 
	virtual void receiveDataToQueue() override;
	virtual void receiveData(std::string& out, int cs) override;
    std::thread acceptThread;
    std::thread recieveThread;
	
    int serverSocket;
    sockaddr_in serverAddress{};
	std::vector<int> clientSockets;
	std::mutex clientSocketsMutex;
	
};

#endif