#include "GameClient.h"

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
	SDL_RenderClear(renderer);
	
	// add stuff to render
	
	
    // Render text using SDL
	
	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);
    const char* text = "Hello, SDL Default Font!";
    SDL_Color textColor = { 255, 0, 0, 255 }; // Red color
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(Sans, text, textColor);
    if (textSurface)
    {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture)
        {
            SDL_Rect textRect = { 100, 100, textSurface->w, textSurface->h }; // Adjust position as needed
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        
        SDL_FreeSurface(textSurface);
    }
    
    
	
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