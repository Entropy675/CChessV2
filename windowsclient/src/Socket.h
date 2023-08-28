#ifndef SOCKET_H
#define SOCKET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <winsock2.h> // Winsock2 header

#include "defs.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

class Socket
{
	public:
	Socket(const char* connect);
	~Socket();
	
	int startConnection();
	
	void sendData(const char*);
	bool receiveData(std::string& out); // returns true if there is data to recieve.
	
	std::string& getIP();
	void setIP(const char*);
	
	bool isConnected();
	
	private:
    WSADATA wsaData;
    SOCKET clientSocket;
	std::string connectIP;
	bool disconnected = true;
	
};



#endif