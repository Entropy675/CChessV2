#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "defs.h"

class Window
{
	public:
	Window(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	~Window();

	SDL_Window* getWindowSDL();
	SDL_Renderer* getRendererSDL();

	// return x & y values, takes fraction of screen. Accounts for different screen sizes with offset internally.
	int getScreenX(float, bool = true); // like 0.5 would be the X value for the halfway of the screen, for 1600 screen it returns 800.
	int getScreenY(float, bool = true);
	
	void toggleFullscreen();

	int windowOffsetX = 0;
	int windowOffsetY = 0;
	private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};



#endif