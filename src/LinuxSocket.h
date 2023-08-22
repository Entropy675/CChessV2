#ifndef LINUXSOCKET_H
#define LINUXSOCKET_H

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
	virtual void receiveDataToQueue(int socket) override;
	virtual void receiveData(std::string& out, int cs) override;
	virtual void resendOrder(int socket, std::string cmd) override;
	virtual void startResendOfOrder(int socket, const std::string& cmd) override;
	
    sockaddr_in serverAddress{};
};

#endif