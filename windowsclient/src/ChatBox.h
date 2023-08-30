#ifndef CHATBOX_H
#define CHATBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "defs.h"
#include "Window.h"
#include "Text.h"

#include <stack>
#include <deque>

class ChatBox
{
	public:
	ChatBox(int numtext = 10, int gap = 20);
	~ChatBox();
	
	void addText(const char*);
	void moveUp();
	void moveDown();
	
	void draw();
	void update();
	
	
	private:
	std::stack<Text*> top;
	std::deque<Text*> buff; // displayed to screen
	std::stack<Text*> bot; 
	
	int numTextDisplayable;
	int gapBetweenText;
};


#endif