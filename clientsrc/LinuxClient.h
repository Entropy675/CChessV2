#ifndef LINUXCLIENT
#define LINUXCLIENT

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "defs.h"

class LinuxClient()
{
	public:
	int startConnection();

	private:
    int clientSocket;
    sockaddr_in serverAddress{};
}

#endif