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
#include <iostream>

class ChatBox
{
	public:
	ChatBox(SDL_Renderer* = nullptr, Window* = nullptr, int numtext = 10, int gap = 30, int x = 0, int y = 0);
	~ChatBox();
	
	void addText(const char*);
	void moveUp();
	void moveDown();
	
	// takes in % of screen values, puts the top left corner of the chatbox at that location
	void setScreenPos(float screenX, float screenY);
	
	void draw();
	void update();
	
	
	private:	
	Window* win;
	SDL_Renderer* renderer;
	
	int x, y;
	
	// IDEA: 
	// top stack = text off the top of the screen
	// bot stack = text off the bottom of the screen
	// buff deque = text displayed to the screen.
	// When we scroll we pop from one of the stack & push to one side of deque, 
	// and push to other stack and pop from the other side of the deque.
	// draw() draws all the texts. Update() updates them to their appropriate positions.
	std::stack<Text*> top;
	std::deque<Text*> buff; // displayed to screen
	std::stack<Text*> bot; 
	
	int numTextDisplayable;
	int gapBetweenText;
};


#endif