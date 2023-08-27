#include "GameClient.h"
#include <iostream>
#include <cstdlib>

GameClient::GameClient()
	: soc(CONNECT_IP) {}

GameClient::~GameClient()
{
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

	
	TTF_Init();
    font = TTF_OpenFont("FreeSans.ttf", 14);
	    if (font == NULL) {
        SDL_Log("GameClient::init(): font not found\n");
        exit(EXIT_FAILURE);
    }
	
	
	getTextureAndRectLine(renderer, getScreenX(0.705), getScreenY(0.01), inputText.c_str(), font, &textTexture, &textRect);
	
	// chessboard tile size
	sqWidth = (int)(SCREEN_WIDTH*0.7)/MAX_ROW_COL - 1;
	sqHeight = (int)(SCREEN_HEIGHT)/MAX_ROW_COL - 1; 
	
	std::cout << "GameClient::init(): Chessboard Tile - Width: " << sqWidth << "Height: " << sqHeight << std::endl;
	
	running = true;
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
				if(soc.startConnection())
					SDL_Log("GameClient::handleEvents(): Could not connect to the socket server.");
				else
					SDL_Log("GameClient::handleEvents(): Connected.");
			}	
			else if(event.key.keysym.sym == SDLK_t)
			{
				SDL_Log("GameClient::handleEvents(): Attempting to send test packet.");
				std::string testing = "TEST PACKET " + std::to_string(tempCounter++);
				soc.sendData(testing.c_str());
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
	if(soc.receiveData(inputText))
	{
		getTextureAndRectLine(renderer, getScreenX(0.705), getScreenY(0.01), inputText.c_str(), font, &textTexture, &textRect);
	}
	
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
		/* Use TTF textures. */
		

	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
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

int GameClient::startConnection()
{
	return soc.startConnection();
}

std::string& GameClient::getIP()
{
	return soc.getIP();
}

void GameClient::setIP(const char* c)
{
	soc.setIP(c);
}
	
void GameClient::sendData(const char* message)
{
	std::cout << "GameClient::sendData(): Sending: " << (soc.isConnected() ? message : "") << std::endl;
    soc.sendData(message);
}

bool GameClient::receiveData(std::string& out)
{
	return soc.receiveData(out);
}


void GameClient::getTextureAndRectLine(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{
	SDL_Surface *surface;
	SDL_Color textColor = {0, 0, 0, 0};

	// Split the text into lines based on newline characters
	std::vector<std::string> lines;
	std::istringstream textStream(text);
	std::string line;
	while (std::getline(textStream, line))
	{
		lines.push_back(line);
	}

	// Calculate the total height and maximum width of the multiline text
	int totalHeight = 0;
	int maxWidth = 0;
	
	surface = TTF_RenderText_Blended(font, lines[0].c_str(), textColor);
	
	totalHeight = surface->h * lines.size();
	maxWidth = 200;

	SDL_FreeSurface(surface);

	// Create a texture that fits the multiline text
	surface = SDL_CreateRGBSurface(0, maxWidth, totalHeight, 32, 0, 0, 0, 0);
	SDL_Rect destinationRect = {0, 0, 0, 0};

	for (const auto &line : lines)
	{
		surface = TTF_RenderText_Blended(font, line.c_str(), textColor);
		*texture = SDL_CreateTextureFromSurface(renderer, surface);

		destinationRect.w = surface->w;
		destinationRect.h = surface->h;

		SDL_RenderCopy(renderer, *texture, NULL, &destinationRect);

		destinationRect.y += surface->h;
		
		SDL_FreeSurface(surface);
	}

	rect->x = x;
	rect->y = y;
	rect->w = maxWidth;
	rect->h = totalHeight;
}

/*
void GameClient::getTextureAndRectLine(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) 
{
	int text_width;
	int text_height;
	SDL_Surface *surface;
	SDL_Color textColor = {0, 0, 0, 0};

	surface = TTF_RenderText_Blended(font, text, textColor);
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);
	rect->x = x;
	rect->y = y;
	rect->w = text_width;
	rect->h = text_height;
}
*/

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
