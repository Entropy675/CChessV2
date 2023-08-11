#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "defs.h"

class Game
{

public:	
	Game();
	~Game();
	
	// returns flags, 0 for success.
	int init(const char* title, int xpos = SDL_WINDOWPOS_CENTERED, int ypos = SDL_WINDOWPOS_CENTERED, int width = 800, int height = 800, bool fullscreen = false);
	
	void startGameLoop();
	
	void handleEvents();
	void update();
	void render();
	void clean();
	
	void log(const char*);
	bool isRunning();
	
	void toggleFullscreen();
	
private:
	bool running = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};




#endif