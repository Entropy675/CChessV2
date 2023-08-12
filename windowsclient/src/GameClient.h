#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "defs.h"

class GameClient
{

public:	
	GameClient();
	~GameClient();
	
	// returns flags, 0 for success.
	int init(const char* title, int xpos = SDL_WINDOWPOS_CENTERED, int ypos = SDL_WINDOWPOS_CENTERED, int width = 800, int height = 800, bool fullscreen = false);
	
	void startGameLoop();
	
	void handleEvents();
	void update();
	void render();
	void clean();
	
	// return x & y values, takes fraction of screen. Accounts for different screen sizes with offset internally.
	int getScreenX(float, bool = true); // like 0.5 would be the X value for the halfway of the screen, for 1600 screen it returns 800.
	int getScreenY(float, bool = true);

	void log(const char*);
	bool isRunning();
	
	void toggleFullscreen();
	
private:
	int windowOffsetX = 0;
	int windowOffsetY = 0;
	
	int sqWidth = 0;
	int sqHeight = 0;
	
	bool running = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};




#endif