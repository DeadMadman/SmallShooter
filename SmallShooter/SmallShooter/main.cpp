#include <chrono>
#include <SDL.h>
#include <vector>


#include "Engine.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Input.h"

int main(int argc, char* args[])
{
	SDL_Rect spriteSrc{ 0, 0, 16, 9 };
	float scale = 4;Your location
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

	//create entities
	EntityManager entityManager;
	Entity* e = entityManager.createEntity();
	entityManager.setPositionField(e, {10.0f, height * 0.5f + 8});
	entityManager.setSourceField(e, spriteSrc);
	
	for (int i = 0; i < 50000; ++i) {
		Entity* e = entityManager.createEntity();
		entityManager.setPositionField(e, {10.0f, 10.0f});
		entityManager.setSourceField(e, spriteSrc);
	}
	
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
			for (auto e : entityManager.entities) {
				if (e.hasComponent(Entity::Components::POSITION)) {
					updatePosition(entityManager.positions[e.index]);
				}
			}
			for (auto e : entityManager.entities) {
				if (e.hasComponent(Entity::Components::SCR)) {
					engine.drawTexture(entityManager.sources[e.index], {
						entityManager.positions[e.index].x, entityManager.positions[e.index].y,
						entityManager.sources[e.index].w * scale, entityManager.sources[e.index].h * scale});
				}
			}
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
