#include "Text.h"


Text::Text(SDL_Renderer* ren, Window* w)
	: renderer(ren), win(w)
{
	font = TTF_OpenFont("FreeSans.ttf", 14);
	if (font == NULL) 
	{
		SDL_Log("GameClient::init(): font not found\n");
		exit(EXIT_FAILURE);
	}
}

Text::~Text()
{
	
}

void Text::update()
{
	getTextureAndRectLine(renderer, win->getScreenX(0.705), win->getScreenY(0.01), localText.c_str(), font, &textTexture, &textRect);
}

void Text::draw()
{
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

void Text::getTextureAndRectLine(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
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