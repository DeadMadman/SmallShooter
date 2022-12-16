#include <chrono>
#include <SDL.h>
#include <vector>


#include "Engine.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Input.h"

void spawnEnemy(SDL_Rect enemySpriteSrc, int scale, int height, Vector2& enemyPosition, EntityManager& entityManager) {
	Entity e = entityManager.getPooledEntity();
	enemyPosition.y = rand() % height - enemySpriteSrc.h * scale;
	entityManager.poolEnemy(e, enemySpriteSrc, enemyPosition, scale);
}

int main(int argc, char* args[])
{
	SDL_Rect playerSpriteSrc{ 0, 8, 15, 8+4 };
	SDL_Rect enemySpriteSrc{ 0, 0, 15, 8 };
	SDL_Rect bulletSpriteSrc{ 0, 16+4, 4, 4 };
	
	int scale = 4;
	int margin = 48;
	int width = 600;
	int height = 600;
	SDL_Rect bounds = {-margin, -margin, width + margin, height + margin};


	Vector2 enemyPosition = {width, height};
	int spawnTimer = 500;
	
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
	for (int i = 0; i < 3000; ++i) {
		entityManager.createPool();
	}

	Entity e = entityManager.getPooledEntity();
	entityManager.poolPlayer(e, playerSpriteSrc, {10, height / 2 + 8}, scale);
	
	for (int i = 0; i < 20; ++i) {
		spawnEnemy(enemySpriteSrc, scale, height, enemyPosition, entityManager);
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
		
		inputController.update(frameTime);
		engine.render();

		if (currentState == State::GAME) {
			Vector2 dir{0, 0};
			int speed = 100;
			if(inputController.isDown(inputController.keys.moveUp)) {
				dir.y -= speed;
			}
			if(inputController.isDown(inputController.keys.moveDown)) {
				dir.y += speed;
			}
			if(inputController.isDown(inputController.keys.shoot)) {
				entityManager.updateShooting(frameTime, bulletSpriteSrc, scale);
			}
			entityManager.updateInput(dir);
			
			static Uint64 tp = 0;
			if (tp < SDL_GetTicks64()) {
				tp = SDL_GetTicks64() + spawnTimer;
				for (int i = 0; i < 10; ++i) {
					spawnEnemy(enemySpriteSrc, scale, height, enemyPosition, entityManager);
				}
			}
			entityManager.updatePositions(frameTime, bounds, scale);
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
