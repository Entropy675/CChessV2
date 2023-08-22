#ifndef WINDOWSSOCKET_H
#define WINDOWSSOCKET_H


// Windows
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "SocketHandler.h"	

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
	virtual void receiveDataToQueue(int socket) override; // thread
	virtual void receiveData(std::string& out, int cs) override;
	virtual void resendOrder(int socket, std::string cmd) override; // thread, detached
	virtual void startResendOfOrder(int socket, const std::string& cmd) override;

	bool closeDisconnectedSockets(int soc);

	
	WSADATA wsaData;
};




#endif