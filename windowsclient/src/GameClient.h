#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <winsock2.h> // Winsock2 header

#include "defs.h"
#include <string>

class GameClient
{
public:	
	GameClient();
	~GameClient();
	
	// returns flags, 0 for success.
	int init(const char* title, int xpos = SDL_WINDOWPOS_CENTERED, int ypos = SDL_WINDOWPOS_CENTERED, int width = 800, int height = 800, bool fullscreen = false);
	
	void startGameLoop();
	bool isRunning();
	
	std::string& getIP();
	void setIP(const char*);
	
	void log(const char*);
	
private:
	void handleEvents();
	void update();
	void render();
	void clean();
	
	void get_text_and_rect(SDL_Renderer*, int, int, const char*, TTF_Font*, SDL_Texture**, SDL_Rect*);
	
	// return x & y values, takes fraction of screen. Accounts for different screen sizes with offset internally.
	int getScreenX(float, bool = true); // like 0.5 would be the X value for the halfway of the screen, for 1600 screen it returns 800.
	int getScreenY(float, bool = true);

	void toggleFullscreen();
	
	int startConnection();
	void sendData(const char*);
	bool receiveData(std::string& out); // returns true if there is data to recieve.
	
	int windowOffsetX = 0;
	int windowOffsetY = 0;
	
	int tempCounter = 0;
	
	int sqWidth = 0;
	int sqHeight = 0;
	
	bool disconnected = true;
	bool running = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	
	std::string inputText = "well this better be something for testing";
	
    WSADATA wsaData;
    SOCKET clientSocket;
	std::string connectIP;
	
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Texture* textTexture;
    SDL_Texture *texture1;
	
    SDL_Rect textRect;
};




#endif