#include <chrono>
#include <SDL.h>

#include "Engine.h"
#include "Input.h"

int main(int argc, char* args[])
{
	SDL_Rect spriteSrc{ 0, 0, 16, 9 };
	float scale = 4;
	float margin = 48;
	int width = 600;
	int height = 600;
	
	Engine engine{width, height};
	engine.loadTexture("Warship.png");
	engine.loadFont("AsepriteFont.ttf", 16 * scale);
	engine.setText("Press space");
	
	Input inputController{};
	
	bool quit = false;
	auto currentTime = std::chrono::high_resolution_clock::now();

	enum class State {
		START,
		GAME,
		LOSE
	};
	State currentState = State::START;

	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
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
			default: ;
			}
		}
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;
		frameTime = std::min(frameTime, 0.5f);
		
		inputController.update(frameTime);

		engine.render();

		if (currentState == State::GAME) {
			engine.drawTexture(spriteSrc, {25, 25, spriteSrc.w * scale, spriteSrc.h * scale});
			
		}
		else {
			if(inputController.isDown(inputController.keys.shoot)) {
				if (currentState == State::START) {
					currentState = State::GAME;
				}
				else {
					quit = true;
				}
			}
			engine.drawText(width / 2, height / 2);
		}
		engine.present();
		SDL_Delay(16);
	}
   
	engine.cleanup();
    
	return 0;
}
