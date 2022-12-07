#include <chrono>
#include <SDL.h>
#include <vector>


#include "Engine.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Input.h"

int main(int argc, char* args[])
{
	SDL_Rect playerSpriteSrc{ 0, 0, 16, 9 };
	SDL_Rect enemySpriteSrc{ 0, 9, 16, 9 };
	SDL_Rect bulletSpriteSrc{ 0, 36, 9, 9 };
	
	float scale = 4;
	float margin = 48;
	int width = 600;
	int height = 600;
	SDL_FRect bounds = {-margin, -margin, width + margin, height + margin};
	
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
	for (int i = 0; i < 200; ++i) {
		entityManager.createPool();
	}

	Entity* e = entityManager.getPooledEntity();
	entityManager.poolPlayer(e, playerSpriteSrc, height);
	
	for (int i = 0; i < 20; ++i) {
		e = entityManager.getPooledEntity();
		entityManager.createEnemy(e, enemySpriteSrc, width, i);
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
			Vector2 dir{0, 0};
			float speed = 100;
			if(inputController.isDown(inputController.keys.moveUp)) {
				dir.y -= speed;
			}
			if(inputController.isDown(inputController.keys.moveDown)) {
				dir.y += speed;
			}
			if(inputController.isDown(inputController.keys.shoot)) {
				entityManager.updateShooting(frameTime, bulletSpriteSrc);
			}
			entityManager.updateInput(dir);
			entityManager.updatePositions(frameTime, bounds);
			entityManager.collide();
			entityManager.renderEntities(engine, scale);
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
