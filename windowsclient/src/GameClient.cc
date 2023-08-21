#include "GameClient.h"
#include <iostream>
#include <cstdlib>

GameClient::GameClient()
	: connectIP(CONNECT_IP)
{
	TTF_Init();
}

GameClient::~GameClient()
{
	
    closesocket(clientSocket);

    // Cleanup Winsock
    WSACleanup();
	
    TTF_Quit();
	// clean up SDL
	clean();
}

int GameClient::init(const char* title, int xpos, int ypos, int width, int height, bool fs)
{
	// Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
	{
        SDL_Log("GameClient::init(): SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }
	
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
        SDL_Log("GameClient::init(): Window could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }
	
	// windows created
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer)
	{
        SDL_Log("GameClient::init(): Renderer could not be created! SDL_Error: %s", SDL_GetError());
		return 1;
	}	
	
	// initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "GameClient::init(): Failed to initialize Winsock." << std::endl;
        return 1;
    }
	
	// chessboard tile size
	sqWidth = (int)(SCREEN_WIDTH*0.7)/MAX_ROW_COL - 1;
	sqHeight = (int)(SCREEN_HEIGHT)/MAX_ROW_COL - 1; 
	
	std::cout << "GameClient::init(): Chessboard Tile - Width: " << sqWidth << "Height: " << sqHeight << std::endl;
	
	running = true;
	return 0;
}

int GameClient::startConnection()
{
	if(!disconnected)
	{
		closesocket(clientSocket);
		disconnected = true;
		return 1;
	}
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "GameClient::startConnection(): Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(connectIP.c_str());

    // Connect to server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
	{
        std::cout << "GameClient::startConnection(): Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

	disconnected = false;
    std::cout << "GameClient::startConnection(): Connected to the server." << std::endl;
	return 0;
}

void GameClient::startGameLoop()
{
	if(!running)
	{
		SDL_Log("GameClient::startGameLoop(): The GameClient object has not been initialized.");
	}
	
	while(running)
	{
		handleEvents();	// handle user input
		update();		// update all objects
		render();		// render changes to display
	}
	
	SDL_Log("GameClient::startGameLoop(): Exiting GameClient loop...");
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
		
			SDL_Log("GameClient::handleEvents(): Key pressed: %d\n", event.key.keysym.sym);
			
			if (event.key.keysym.sym == SDLK_SPACE) 
			{
				SDL_Log("GameClient::handleEvents(): Space button pressed.");
			}
			else if(event.key.keysym.sym == SDLK_BACKQUOTE)
			{
				SDL_Log("GameClient::handleEvents(): Tilde ~ button pressed.");
				toggleFullscreen();
			}
			else if(event.key.keysym.sym == SDLK_c)
			{
				if(startConnection())
					SDL_Log("GameClient::handleEvents(): Could not connect to the socket server.");
				else
					SDL_Log("GameClient::handleEvents(): Connected.");
			}	
			else if(event.key.keysym.sym == SDLK_t)
			{
				SDL_Log("GameClient::handleEvents(): Attempting to send test packet.");
				std::string testing = "TEST PACKET " + std::to_string(tempCounter++);
				sendData(testing.c_str());
			}
			break;
		
		case SDL_TEXTINPUT:
			inputText += event.text.text;
			break; 
		
		default:	
			break;
	}
}

void GameClient::update()
{
	return;
}

int GameClient::getScreenX(float percent, bool topLeft)
{
	if(topLeft)
		return (int)(SCREEN_WIDTH*percent) + windowOffsetX;
	return (int)(SCREEN_WIDTH*percent) - windowOffsetX;
}
int GameClient::getScreenY(float percent, bool topLeft)
{
	if(topLeft)
		return (int)(SCREEN_HEIGHT*percent) + windowOffsetY;
	return (int)(SCREEN_HEIGHT*percent) - windowOffsetY;
}

void GameClient::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	// add stuff to render
	
	// Chessboard draw
	for(int x = 0; x < MAX_ROW_COL; ++x)
	{
		for(int y = 0; y < MAX_ROW_COL; ++y)
		{
			// RAINBOW!!
			// SDL_SetRenderDrawColor(renderer, x*(255/MAX_ROW_COL), y*(255/MAX_ROW_COL), 255 - x*(255/MAX_ROW_COL/2) - y*(255/MAX_ROW_COL/2), 255);
			if((y+x) % 2 == 0)
				SDL_SetRenderDrawColor(renderer, 118, 150, 86, 255);
			else
				SDL_SetRenderDrawColor(renderer, 238, 238, 210, 255);
			SDL_Rect tile = {(int)(x*sqWidth + getScreenX(0.005) + 0.005), (int)(y*sqHeight + getScreenY(0.005) + 0.005), (int)(sqWidth - 0.005), (int)(sqHeight - 0.005)};
			SDL_RenderFillRect(renderer, &tile);    // Draw the tiles
		}
	}
	
	SDL_Rect rect;


	// OUTLINES
    // Set the draw color for the rectangle - Draw the screen outline
    // Create rectangles for chatbox
	// (x, y, width, height)
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    rect = {getScreenX(0.705), getScreenY(0.01), getScreenX(0.29), getScreenY(0.98, false)};
    SDL_RenderFillRect(renderer, &rect);
	
	// Load a font (replace with the actual path to your font file)
	
	// Render the text to the screen
	/*
	textSurface = TTF_RenderText_Solid(font, inputText.c_str(), textColor);
	rect = { 10, 10, 300, 300};
	if (!textSurface) 
	{
		SDL_Log("Error creating text surface: %s %s", TTF_GetError(), inputText.c_str());
		// Handle the error appropriately
	}
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, textTexture, nullptr, &rect);
	*/
	
	SDL_RenderPresent(renderer);
}

std::string& GameClient::getIP()
{
	return connectIP;
}

void GameClient::setIP(const char* inp)
{
	connectIP = inp; 
}

void GameClient::clean()
{
	SDL_Log("GameClient::clean(): Cleaning...");
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool GameClient::isRunning()
{
	return running;
}

void GameClient::sendData(const char* message)
{
	std::cout << "GameClient::sendData(): Sending: " << (disconnected ? "" : message) << std::endl;
    if (send(clientSocket, disconnected ? "" : message, disconnected ? 0 : strlen(message), 0) == SOCKET_ERROR) 
	{
        std::cout << "GameClient::sendData(): Failed to send data. Error code: " << WSAGetLastError() << std::endl;
        // fail code goes here
    }
}

void GameClient::toggleFullscreen()
{
	Uint32 flags = SDL_GetWindowFlags(window);
	int screenWidth, screenHeight;
	
    if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) 
	{
		SDL_SetWindowFullscreen(window, 0);
		render();		// render changes to display
		windowOffsetX = 0;
		windowOffsetY = 0;
		
        SDL_Log("GameClient::toggleFullscreen(): Window is now in windowed mode.");
    } 
	else 
	{
		// Calculate scaling factors based on screen and image dimensions
		// theoretically you can scale each pixel to the right position with the following code
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		render();		// render changes to display
		SDL_GetWindowSize(window, &screenWidth, &screenHeight);
		windowOffsetX = (int)(std::abs(screenWidth - SCREEN_WIDTH)*0.35);
		windowOffsetY = (int)(std::abs(screenHeight - SCREEN_HEIGHT)*0.35);

        SDL_Log("GameClient::toggleFullscreen(): Window is now in fullscreen mode.");
    }
	
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	std::string s = "GameClient::toggleFullscreen(): Screen Dimensions - X: " + std::to_string(screenWidth) + " Y: " + std::to_string(screenHeight);
	SDL_Log(s.c_str());
}