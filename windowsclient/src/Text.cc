#include "Text.h"


Text::Text(Window* w, const std::string& t)
	: localText(t), win(w)
{
	font = TTF_OpenFont("FreeSans.ttf", 14);
	if (font == NULL) 
	{
		SDL_Log("GameClient::init(): font not found\n");
		exit(EXIT_FAILURE);
	}
	textTexture = SDL_CreateTextureFromSurface(win->getRendererSDL(), surface);
}

Text::~Text()
{
	if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (textTexture)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
	
}

// takes in % of the screen to be placed at, uses windows get screen functions to get the pix value for that
void Text::updateTo(int screenX, int screenY)
{
	getTextureAndRectLine(win->getRendererSDL(), screenX, screenY, localText.c_str(), font, &textTexture, &textRect);
}

void Text::draw()
{
	SDL_RenderCopy(win->getRendererSDL(), textTexture, NULL, &textRect);
}

void Text::getTextureAndRectLine(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{
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
	
	if(surface)
	{
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
	
	surface = TTF_RenderText_Blended(font, lines[0].c_str(), textColor);
	totalHeight = surface->h * lines.size();
	SDL_FreeSurface(surface);

	// Create a texture that fits the multiline text
	SDL_Surface* surfacemain = SDL_CreateRGBSurface(0, maxWidth, totalHeight, 32, 0, 0, 0, 0);
	SDL_Rect destinationRect  = {0, 0, 0, 0};
	
	for (const auto &line : lines)
	{
		surface = TTF_RenderText_Blended(font, line.c_str(), textColor);
		

		destinationRect.w = surface->w;
		destinationRect.h = surface->h;

		SDL_BlitSurface(surface, NULL, surfacemain, &destinationRect);

		destinationRect.y += surface->h;
		
		SDL_FreeSurface(surface);
	}
	
	if (textTexture)
    {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
	textTexture = SDL_CreateTextureFromSurface(renderer, surfacemain);

	rect->x = x;
	rect->y = y;
	rect->w = maxWidth;
	rect->h = totalHeight;
}
