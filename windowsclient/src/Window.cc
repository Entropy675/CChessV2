#include "Window.h"
#include <iostream>

Window::Window(const char* title, int xpos, int ypos, int width, int height, bool fs)
{

	int flags = SDL_WINDOW_SHOWN;
	if(fs)
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;


    window = SDL_CreateWindow(
        title,
        xpos,
        ypos,
        width,
        height,
        flags
    );

    if (window == nullptr) 
	{
		std::cerr << "Window::Window(): Window could not be created!";
        throw SDL_GetError();
    }
	
	// windows created
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer)
	{
		std::cerr << "Window::Window(): Renderer could not be created!";
        throw SDL_GetError();
	}
	
}

Window::~Window()
{
	std::cerr << "Window::~Window(): Cleaning..." << std::endl;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

SDL_Window* Window::getWindowSDL()
{
	return window;
}

SDL_Renderer* Window::getRendererSDL()
{
	return renderer;
}

int Window::getScreenX(float percent, bool topLeft)
{
	if(topLeft)
		return (int)(SCREEN_WIDTH*percent) + windowOffsetX;
	return (int)(SCREEN_WIDTH*percent) - windowOffsetX;
}

int Window::getScreenY(float percent, bool topLeft)
{
	if(topLeft)
		return (int)(SCREEN_HEIGHT*percent) + windowOffsetY;
	return (int)(SCREEN_HEIGHT*percent) - windowOffsetY;
}	

void Window::toggleFullscreen()
{
	Uint32 flags = SDL_GetWindowFlags(window);
	int screenWidth, screenHeight;
	
    if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) 
	{
		SDL_SetWindowFullscreen(window, 0);
		windowOffsetX = 0;
		windowOffsetY = 0;
		
        SDL_Log("Window::toggleFullscreen(): Window is now in windowed mode.");
    } 
	else 
	{
		// Calculate scaling factors based on screen and image dimensions
		// theoretically you can scale each pixel to the right position with the following code
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetWindowSize(window, &screenWidth, &screenHeight);
		windowOffsetX = (int)(std::abs(screenWidth - SCREEN_WIDTH)*0.35);
		windowOffsetY = (int)(std::abs(screenHeight - SCREEN_HEIGHT)*0.35);

        SDL_Log("Window::toggleFullscreen(): Window is now in fullscreen mode.");
    }
	
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	std::string s = "Window::toggleFullscreen(): Screen Dimensions - X: " + std::to_string(screenWidth) + " Y: " + std::to_string(screenHeight);
	SDL_Log(s.c_str());
}
