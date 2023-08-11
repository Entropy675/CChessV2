#include "Game.h"

Game::Game()
{
	
}

Game::~Game()
{
	clean();
}

int Game::init(const char* title, int xpos, int ypos, int width, int height, bool fs)
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

    if (window == nullptr) {
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
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	running = true;
	return 0;
}

void Game::startGameLoop()
{
	if(!running)
	{
		SDL_Log("startGameLoop ERROR; The game object has not been initialized.");
	}
	
	while(running)
	{
		handleEvents();	// handle user input
		update();		// update all objects
		render();		// render changes to display
	}
	
	SDL_Log("startGameLoop ERROR; Exiting game loop...");
}

void Game::log(const char* s)
{
	SDL_Log(s);
}

void Game::handleEvents()
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

void Game::update()
{
	
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	// add stuff to render
	
	
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_Log("Game cleaning...");
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Game::isRunning()
{
	return running;
}


void Game::toggleFullscreen()
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