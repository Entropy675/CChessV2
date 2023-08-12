#include "GameClient.h"
#include <iostream>
#include <cstdlib>

GameClient::GameClient()
{
	
}

GameClient::~GameClient()
{
	// clean up SDL
	clean();
	
    // Close socket
    closesocket(clientSocket);

    // Cleanup Winsock
    WSACleanup();
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
	
	// initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

	
	std::cout << "X: " << SCREEN_WIDTH*0.8/MAX_ROW_COL << "Y: " << SCREEN_HEIGHT/MAX_ROW_COL << std::endl;
	
	sqWidth = (int)(SCREEN_WIDTH*0.7)/MAX_ROW_COL - 1;
	sqHeight = (int)(SCREEN_HEIGHT)/MAX_ROW_COL - 2; 
	
	running = true;
	return 0;
}

int GameClient::startConnection()
{
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(connectIP.c_str());

    // Connect to server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server." << std::endl;
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
			else if(event.key.keysym.sym == SDLK_c)
			{
				if(startConnection())
					SDL_Log("Could not connect to the socket server.");
			}
			break;
			
		default:
			break;
	}
}

void GameClient::update()
{
	
}

int GameClient::getScreenX(float percent, bool topLeft)
{
	if(topLeft)
		return (int)(SCREEN_WIDTH*percent) + windowOffsetX/2;
	return (int)(SCREEN_WIDTH*percent) - windowOffsetX/2;
}
int GameClient::getScreenY(float percent, bool topLeft)
{
	if(topLeft)
		return (int)(SCREEN_HEIGHT*percent) + windowOffsetY/2;
	return (int)(SCREEN_HEIGHT*percent) - windowOffsetY/2;
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
			SDL_Rect tile = {(int)(x*sqWidth + getScreenX(0.005) + 0.005), (int)(y*sqHeight + getScreenY(0.01) + 0.005), (int)(sqWidth - 0.005), (int)(sqHeight - 0.005)};
			SDL_RenderFillRect(renderer, &tile);    // Draw the tiles
		}
	}
	
	SDL_Rect rect;
	
	// OUTLINES
    // Set the draw color for the rectangle 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    rect = {getScreenX(0.705), getScreenY(0.01), getScreenX(0.99, false), getScreenY(0.98, false)};
    SDL_RenderFillRect(renderer, &rect);    // Draw the screen outline
	
    // Create rectangles for chatbox
	// (x, y, width, height)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    rect = {getScreenX(0.7), getScreenY(0), 20, getScreenY(1.00, false)};
    SDL_RenderFillRect(renderer, &rect);
	
	// (x, y, width, height)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    rect = {getScreenX(0.99), getScreenY(0), 700, getScreenY(1.00, false)};
    SDL_RenderFillRect(renderer, &rect);
	
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
	
    if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) 
	{
        SDL_Log("Window is now not in fullscreen mode.");
		SDL_SetWindowFullscreen(window, 0);
		windowOffsetX = 0;
		windowOffsetY = 0;
    } 
	else 
	{
		// Calculate scaling factors based on screen and image dimensions
		// theoretically you can scale each pixel to the right position with the following code
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

        int screenWidth, screenHeight;
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);

		windowOffsetX = std::abs(screenWidth - SCREEN_WIDTH)/2;
		windowOffsetY = std::abs(screenHeight - SCREEN_HEIGHT)/2;
		
			
		// Clear renderer
        SDL_RenderClear(renderer);

        SDL_Log("Window is now in fullscreen mode.");
		std::string s = "X: " + std::to_string(screenWidth) + " Y: " + std::to_string(screenHeight);
		SDL_Log(s.c_str());
		
    }
}