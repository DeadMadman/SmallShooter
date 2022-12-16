#pragma once
#include <vector>

#include "Engine.h"
#include "Entity.h"

class EntityManager {
public:
    std::vector<Entity> entities{};
    std::vector<std::bitset<Entity::Components::SIZE>> components{};
    
    std::vector<int> healths{};
    std::vector<Vector2> positions{};
    std::vector<Vector2> velocities{};
    std::vector<SDL_Rect> sources{};
    std::vector<SDL_Rect> destinations{};

    std::vector<Player> players{};

    Entity createEntity();
    void setHealthComponent(Entity e, int hp);
    void setPositionComponent(Entity e, Vector2 pos);
    void setVelocityComponent(Entity e, Vector2 vel);
    void setSourceComponent(Entity e, SDL_Rect src);
    void setDestinationComponent(Entity e, SDL_Rect dst);
    void setPlayerComponent(Entity e, Uint64 cool);

    void setCollisionComponent(Entity e);
    void setEnemyComponent(Entity e);
    void setPlayerBulletComponent(Entity e);
    
    void renderEntities(Engine& eng, int scale);
    void updatePositions(float dt, SDL_Rect bounds, int scale);
    void damageEntity(Entity enemy);
    void collide();
    
    void updateInput(Vector2 vel);
    void updateShooting(float dt, SDL_Rect bulletSpriteSrc, int scale);
    
    void createPool();

    Entity getPooledEntity();
    
    void poolPlayer(Entity e,SDL_Rect src, Vector2 pos, int scale);
    void poolEnemy(Entity e,SDL_Rect src, Vector2 pos, int scale);

    void poolBullet(Entity e, SDL_Rect src, Vector2 pos, int scale);
};
