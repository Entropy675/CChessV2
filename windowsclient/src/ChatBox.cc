#include "ChatBox.h"


ChatBox::ChatBox(Window* w, int xstart, int ystart, int numtext, int gap)
	: win(w), x(xstart), y(ystart), numTextDisplayable(numtext), gapBetweenText(gap) 
{
	if(!w)
	{
		std::cerr << "ChatBox::ChatBox(): Renderer or window not passed in! The ChatBox will behave in undefined ways." << std::endl;
	}
	std::cerr << "ChatBox::ChatBox(): Initializing ChatBox..." << std::endl;
}

ChatBox::~ChatBox()
{
	std::cerr << "ChatBox::~ChatBox(): Entering dtor..." << std::endl;
	Text* temp;
	while(!top.empty())
	{
		temp = top.top();
		delete temp;
		top.pop();
	}
	
	for(Text* t : buff)
		delete t;
	
	for(Text* t : bot)
		delete t;
	
	std::cerr << "ChatBox::~ChatBox(): Exiting dtor..." << std::endl;
}

void ChatBox::addText(const std::string& c)
{
	if(buff.size() < numTextDisplayable) // temp early return for testing
		return;
	
	Text* text = new Text(win, c);
	buff.push_back(text);
}

void ChatBox::setScreenPos(float screenX, float screenY)
{
	y = win->getScreenY(screenY);
	x = win->getScreenX(screenX);
}

void ChatBox::draw()
{
	for(int i = 0; i < buff.size(); i++)
		if(buff[i])
			buff[i]->draw();
}

void ChatBox::update()
{
	for(int i = 0; i < buff.size(); i++)
		if(buff[i])
			buff[i]->updateTo(x, y + i*gapBetweenText);
}

void ChatBox::moveUp()
{
	if(buff.size() <= numTextDisplayable) // if we aren't full up on the main buff this is irrelevent
		return;
	
	// this is 
}

void ChatBox::moveDown()
{
	if(buff.size() <= numTextDisplayable) // if we aren't full up on the main buff this won't do anything
		return;
	
}