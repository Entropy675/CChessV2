#include "GameClient.h"


#include <iostream>
#include "defs.h"

int main(int argc, char* argv[]) 
{
	GameClient g;
	
	g.startGameLoop();
	if(g.init("Demo Game Window ;)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, true))
	{
		std::cout << "Failure to initialize window." << std::endl;
		g.log("Failure to initialize window.");
	}

	g.startGameLoop();
	
    return 0;
}
