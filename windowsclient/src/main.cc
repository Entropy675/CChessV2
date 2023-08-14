#include "GameClient.h"


#include <iostream>
#include "defs.h"

int main(int argc, char* argv[]) 
{
	GameClient g;
	if(g.init("Demo Game Window ;)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false))
	{
		std::cout << "STDOUT: Failure to initialize window." << std::endl;
		g.log("SDL_LOG: Failure to initialize window.");
	}

	g.startGameLoop();
	
    return 0;
}
