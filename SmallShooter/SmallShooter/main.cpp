#include <SDL.h>
#include "Engine.h"

int main(int argc, char* args[])
{
	Engine engine{};
	
	SDL_Event e;
	bool quit = false; 

	while (quit == false) { 
		while (SDL_PollEvent(&e)) { 
			if (e.type == SDL_QUIT)
				quit = true; 
		} 
	}
   
	engine.cleanup();
    SDL_Quit();
    
	return 0;
}