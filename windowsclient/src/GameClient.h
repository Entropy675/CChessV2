#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


#include "defs.h"
#include "Socket.h"
#include "Text.h"
#include "Window.h"


#include <vector>
#include <iostream>
#include <sstream>
#include <string>


class GameClient
{
public:	
	GameClient();
	~GameClient();
	
	// returns flags, 0 for success.
	int init(const char* title, int xpos = SDL_WINDOWPOS_CENTERED, int ypos = SDL_WINDOWPOS_CENTERED, int width = 800, int height = 800, bool fullscreen = false);
	
	void startGameLoop();
	bool isRunning();
	
	std::string& getIP();
	void setIP(const char*);
	
	void log(const char*);
	
private:
	void handleEvents();
	void update();
	void render();
	void clean();
	
	void drawBoard();

	Socket soc;
	
	int tempCounter = 0;
	
	int sqWidth = 0;
	int sqHeight = 0;
	
	bool running = false;
	Window* win = nullptr;
	
	std::string inputText = "well this better be something for testing";
};




#endif