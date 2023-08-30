#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "defs.h"
#include "Window.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

class Text
{
	public:
	Text(SDL_Renderer* = nullptr, Window* = nullptr, float x = 0, float y = 0);
	~Text();
	
	void update();
	void draw();
	
	void setScreenY(float);
	void setScreenX(float);
	void setScreenPos(float x, float y);
	
	private:
	void getTextureAndRectLine(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);

	std::string localText;
	
	Window* win;
	SDL_Renderer* renderer;
	
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Texture* textTexture;
    SDL_Texture *texture1;
	
    SDL_Rect textRect;
	
	float screenX, screenY;
};


#endif