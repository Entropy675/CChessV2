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
	Text(Window* = nullptr, const std::string& text = "");
	~Text();
	
	void updateTo(int screenX, int screenY);
	void draw();
	
	private:
	void getTextureAndRectLine(SDL_Renderer* ren, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);

	std::string localText;
	
	Window* win;
	
	TTF_Font* font;
	SDL_Color textColor = {0, 0, 0, 0};
	SDL_Texture* textTexture;
	
    SDL_Rect textRect;
	int maxWidth = 200;
	SDL_Surface* surface;
};


#endif