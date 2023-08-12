#include "GameClient.h"
#include <iostream>

GameClient::GameClient()
{
	
}

GameClient::~GameClient()
{
	clean();
}

int GameClient::init(const char* title, int xpos, int ypos, int width, int height, bool fs)
{
	// Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
        SDL_Log("init ERROR; SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }
	
	int flags = SDL_WINDOW_SHOWN;
	
	if(fs)
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	
	// Create a window
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
        SDL_Log("init ERROR; Window could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }
	
	// windows created
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer)
	{
        SDL_Log("init ERROR; Renderer could not be created! SDL_Error: %s", SDL_GetError());
		return 1;
	}	
	
	std::cout << "X: " << SCREEN_WIDTH*0.8/MAX_ROW_COL << "Y: " << SCREEN_HEIGHT/MAX_ROW_COL << std::endl;
	
	sqWidth = SCREEN_WIDTH*0.8/MAX_ROW_COL - 1;
	sqHeight = SCREEN_HEIGHT/MAX_ROW_COL - 2; 
	
	running = true;
	return 0;
}

void GameClient::startGameLoop()
{
	if(!running)
	{
		SDL_Log("startGameLoop ERROR; The GameClient object has not been initialized.");
	}
	
	while(running)
	{
		handleEvents();	// handle user input
		update();		// update all objects
		render();		// render changes to display
	}
	
	SDL_Log("startGameLoop ERROR; Exiting GameClient loop...");
}

void GameClient::log(const char* s)
{
	SDL_Log(s);
}

void GameClient::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type)
	{
		case SDL_QUIT:
			running = false;
			break;
			
		case SDL_KEYDOWN:
		
			SDL_Log("Key pressed: %d\n", event.key.keysym.sym);
			
			if (event.key.keysym.sym == SDLK_SPACE) 
			{
				SDL_Log("Space button pressed.");
			}
			else if(event.key.keysym.sym == SDLK_BACKQUOTE)
			{
				SDL_Log("Tilde ~ button pressed.");
				toggleFullscreen();
			}
			break;
			
		default:
			break;
	}
}

void GameClient::update()
{
	
}

void GameClient::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	// add stuff to render
	
	for(int x = 0; x < MAX_ROW_COL; ++x)
	{
		for(int y = 0; y < MAX_ROW_COL; ++y)
		{
			SDL_SetRenderDrawColor(renderer, x*(255/MAX_ROW_COL), y*(255/MAX_ROW_COL), 255 - x*(255/MAX_ROW_COL/2) - y*(255/MAX_ROW_COL/2), 255);
			SDL_Rect tile = {(int)(x*sqWidth + SCREEN_WIDTH*0.005 + 0.005), (int)(y*sqHeight + SCREEN_HEIGHT*0.01 + 0.005), (int)(sqWidth - 0.005), (int)(sqHeight - 0.005)};
			SDL_RenderFillRect(renderer, &tile);    // Draw the screen outline
		}
	}
	
	
	
	
	// OUTLINES
    // Set the draw color for the rectangle (red in this case)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect1 = {(int)(SCREEN_WIDTH*0.805), (int)(SCREEN_HEIGHT*0.01), (int)(SCREEN_WIDTH*0.99), (int)(SCREEN_HEIGHT*0.98)};
    SDL_RenderFillRect(renderer, &rect1);    // Draw the screen outline
	
    // Create a rectangle
	// (x, y, width, height)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect2 = {(int)(SCREEN_WIDTH*0.8), (int)(SCREEN_HEIGHT*0), (int)(SCREEN_WIDTH*0.005), (int)(SCREEN_HEIGHT)};
    SDL_RenderFillRect(renderer, &rect2);
	
    
	SDL_RenderPresent(renderer);
}

void GameClient::clean()
{
	SDL_Log("GameClient cleaning...");
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool GameClient::isRunning()
{
	return running;
}


void GameClient::toggleFullscreen()
{
	Uint32 flags = SDL_GetWindowFlags(window);
	
    if (flags & SDL_WINDOW_FULLSCREEN) 
	{
        SDL_Log("Window is now not in fullscreen mode.");
		SDL_SetWindowFullscreen(window, 0);
    } 
	else 
	{
        SDL_Log("Window is now in fullscreen mode.");
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
}