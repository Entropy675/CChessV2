#include "GameClient.h"
#include <iostream>
#include <cstdlib>

GameClient::GameClient()
	: soc(CONNECT_IP) {}

GameClient::~GameClient()
{
	std::cout << "GameClient::~GameClient(): Entering GameClient dtor..." << std::endl;
	
    TTF_Quit();
	
	if(chat)
		delete chat;
	else
		std::cout << "GameClient::~GameClient(): No chat pointer..." << std::endl;
	
	if(win)
		delete win;
	else
		std::cout << "GameClient::~GameClient(): No window pointer..." << std::endl;
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
	
	TTF_Init();
	
	win = new Window(title, xpos, ypos, width, height, fs);
	
	//getTextureAndRectLine(win->getRendererSDL(), win->getScreenX(0.705), win->getScreenY(0.01), inputText.c_str(), font, &textTexture, &textRect);
	// text.update();
	
	// chessboard tile size
	sqWidth = (int)(SCREEN_WIDTH*0.7)/MAX_ROW_COL - 1;
	sqHeight = (int)(SCREEN_HEIGHT)/MAX_ROW_COL - 1; 
	
	std::cout << "GameClient::init(): Chessboard Tile - Width: " << sqWidth << "Height: " << sqHeight << std::endl;
	
	chat = new ChatBox(win, 300, 300);
	chat->addText(std::string("Testing #1 some random text"));
	chat->addText(std::string("Testing #2 and again, and again"));
	chat->addText(std::string("Testing #3 And finally lmao"));
	
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
				win->toggleFullscreen();
				render();
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
		//getTextureAndRectLine(win->getRendererSDL(), win->getScreenX(0.705), win->getScreenY(0.01), inputText.c_str(), font, &textTexture, &textRect);
		// make a new text
		chat->addText(inputText);
		chat->update();
	}
	
	return;
}


void GameClient::render()
{
    SDL_SetRenderDrawColor(win->getRendererSDL(), 0, 0, 0, 255);
	SDL_RenderClear(win->getRendererSDL());
	
	// add stuff to render
	// Chessboard draw
	drawBoard();

    // Create rectangles for chatbox (might wanna turn this into some kind of object that handles this, with two stacks for top and bottom to allow scroll)
	SDL_Rect rect;
    SDL_SetRenderDrawColor(win->getRendererSDL(), 200, 200, 200, 255);
    rect = {win->getScreenX(0.705), win->getScreenY(0.01), win->getScreenX(0.29), win->getScreenY(0.98, false)};
    SDL_RenderFillRect(win->getRendererSDL(), &rect);

	//SDL_RenderCopy(win->getRendererSDL(), textTexture, NULL, &textRect);
	// text.draw();
	if(running)
	{
		chat->draw();
	}
	
	SDL_RenderPresent(win->getRendererSDL());
}

void GameClient::drawBoard()
{
	// Chessboard draw
	for(int x = 0; x < MAX_ROW_COL; ++x)
	{
		for(int y = 0; y < MAX_ROW_COL; ++y)
		{
			// RAINBOW!!
			// SDL_SetRenderDrawColor(win->getRendererSDL(), x*(255/MAX_ROW_COL), y*(255/MAX_ROW_COL), 255 - x*(255/MAX_ROW_COL/2) - y*(255/MAX_ROW_COL/2), 255);
			if((y + x) % 2 == 0)
				SDL_SetRenderDrawColor(win->getRendererSDL(), 150, 77, 34, 255);
			else
				SDL_SetRenderDrawColor(win->getRendererSDL(), 238, 220, 151, 255);
			
			SDL_Rect tile = {(int)(x*sqWidth + win->getScreenX(boardX) + tileGapWidth), (int)(y*sqHeight + win->getScreenY(boardY) + tileGapWidth), (int)(sqWidth - tileGapWidth), (int)(sqHeight - tileGapWidth)};
			SDL_RenderFillRect(win->getRendererSDL(), &tile);    // Draw the tiles
		}
	}
	
}


void GameClient::clean()
{
	SDL_Log("GameClient::clean(): Cleaning...");
	SDL_Quit();
}

bool GameClient::isRunning()
{
	return running;
}

std::string& GameClient::getIP()
{
	return soc.getIP();
}

void GameClient::setIP(const char* c)
{
	soc.setIP(c);
}

