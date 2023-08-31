#include "ChatBox.h"


ChatBox::ChatBox(SDL_Renderer* ren, Window* w, int numtext, int gap, int xstart, int ystart)
	: renderer(ren), win(w), x(xstart), y(ystart), numTextDisplayable(numtext), gapBetweenText(gap) 
{
	if(!ren || !w)
	{
		std::cerr << "ChatBox::ChatBox(): Renderer or window not passed in! The ChatBox will behave in undefined ways." << std::endl;
	}
}

ChatBox::~ChatBox()
{
	
}

void ChatBox::addText(const char* c)
{
	
}

void ChatBox::setScreenPos(float screenX, float screenY)
{
	y = win->getScreenY(screenY);
	x = win->getScreenX(screenX);
}

void ChatBox::draw()
{
	for(Text* text : buff)
	{
		
	}
}

void ChatBox::update()
{
	
}

void ChatBox::moveUp()
{
	
}

void ChatBox::moveDown()
{
	
}