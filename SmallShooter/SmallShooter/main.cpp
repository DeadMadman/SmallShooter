#include <chrono>
#include <SDL.h>
#include "Engine.h"
#include "Input.h"

int main(int argc, char* args[])
{
	Engine engine{640, 480};
	Input inputController{};
	
	bool quit = false; 
	SDL_Event e;

	auto currentTime = std::chrono::high_resolution_clock::now();
	while (!quit) { 
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN: 
				inputController.updateKey(e.key.keysym.scancode, true);
				break;
			case SDL_KEYUP:
				inputController.updateKey(e.key.keysym.scancode, false);
				break;
			}
			engine.update();
		}
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;
		frameTime = std::min(frameTime, 0.5f);
		inputController.update(frameTime);
	}
   
	engine.cleanup();
    SDL_Quit();
    
	return 0;
}
