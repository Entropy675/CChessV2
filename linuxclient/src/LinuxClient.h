#ifndef LINUXCLIENT
#define LINUXCLIENT

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <regex>

#include "LocalBoard.h"
#include "Pos.h"
#include "defs.h"

class LinuxClient
{
	public:
	LinuxClient();
	~LinuxClient();
	
	
	void startGameLoop();
	void toggleSize(); // large/small
	void userInput(std::string&);
	void print(const std::string&);
	void log(const char*);
	
	void printAt(int x, int y, const std::string& s) const;
	
	private:
	LocalBoard game;
	
	// SOCKETS
	int startConnection();
    sockaddr_in serverAddress{};
    int clientSocket;
	std::string connectIP;
	
	// NCURSES
	void initNcurses();
	void cleanupNcurses();
	
	void moveToInputPos() const;

	void drawBoard();
	void drawPieces();
	void drawPieceBar();

	void wideChessConversion(char ch, cchar_t& c);

	void refreshScreen();

	bool largeBoard = false;
	int logwinX = 36;
	int logwinY = 0;
	int logwinWidth = 100;
	int logwinHeight = 20;	
	Pos sqSize; // size of a singe square on board
	Pos offset; // offset of where the center of that square is

	// line characters for drawing board
	cchar_t li;
	cchar_t ld;
	
	WINDOW* logwin;
	const int baseWriteHead;
	int writeHead;
	std::ofstream logfile;
	std::string logstring;
};

#endif