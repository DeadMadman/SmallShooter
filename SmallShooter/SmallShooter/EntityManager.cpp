#include "EntityManager.h"

Entity EntityManager::createEntity() {
    entities.emplace_back(entities.size());
    components.push_back(0);
    
    healths.push_back(0);
    positions.push_back({0, 0});
    velocities.push_back({0, 0});
    sources.push_back({0,0,0,0});
    destinations.push_back({0, 0, 0, 0});

    players.push_back({});
    return entities[entities.size() - 1];
}

void EntityManager::setHealthComponent(Entity e, int hp) {
    components[e.index] |= Entity::Components::HEALTH;
    healths[e.index] = hp;
}

void EntityManager::setPositionComponent(Entity e, Vector2 pos) {
    components[e.index] |= Entity::Components::POSITION;
    positions[e.index] = pos;
}

void EntityManager::setVelocityComponent(Entity e, Vector2 vel) {
    components[e.index] |= Entity::Components::VELOCITY;
    velocities[e.index] = vel;
}

void EntityManager::setSourceComponent(Entity e, SDL_Rect src) {
    components[e.index] |= Entity::Components::SCR;
    sources[e.index] = src;
}

void EntityManager::setDestinationComponent(Entity e, SDL_Rect dst) {
    components[e.index] |= Entity::Components::DST;
    destinations[e.index] = dst;
}

void EntityManager::setCollisionComponent(Entity e) {
    components[e.index] |= Entity::Components::COLLISION;
}

void EntityManager::setPlayerComponent(Entity e, Uint64 cool) {
    components[e.index] |= Entity::Components::PLAYER;
    players[e.index].shootingCooldown = cool;
}

void EntityManager::setEnemyComponent(Entity e) {
    components[e.index] |= Entity::Components::ENEMY;
}

void EntityManager::setPlayerBulletComponent(Entity e) {
    components[e.index] |= Entity::Components::PLAYER_BULLET;
}

void EntityManager::renderEntities(Engine& eng, int scale) {
    std::vector<Entity> entitiesVector;
    for (auto& e : entities) {
        if (e.hasComponent(this, Entity::Components::SCR) && e.hasComponent(this, Entity::Components::DST)) {
            entitiesVector.push_back(e);
        }
    }
    for (auto& e : entitiesVector) {
        eng.drawTexture(sources[e.index], destinations[e.index]);
    }
}

void EntityManager::updatePositions(float dt, SDL_Rect bounds, int scale) {
    std::vector<Entity> entitiesVector;
    for (auto& e : entities) {
        if (e.hasComponent(this, Entity::Components::POSITION) && e.hasComponent(this, Entity::Components::VELOCITY)) {

            Vector2 pos = positions[e.index];
            if (pos.x <= bounds.x || pos.x >= bounds.w || pos.y <= bounds.y || pos.y >= bounds.h) {
                damageEntity(e);
            }
            
            entitiesVector.push_back(e);
        }
    }
    for (auto& e : entitiesVector) {
        updatePosition( positions[e.index], velocities[e.index], dt);
    }

    for (auto& e : entitiesVector) {
        destinations[e.index].x = positions[e.index].x;
        destinations[e.index].y = positions[e.index].y;
        destinations[e.index].w = sources[e.index].w * scale;
        destinations[e.index].h = sources[e.index].h * scale;
    }
}

void EntityManager::damageEntity(Entity enemy) {
    if (enemy.hasComponent(this,Entity::HEALTH)) {
        healths[enemy.index]--; 
        if (healths[enemy.index] <= 0) {
            components[enemy.index] = 0;
        }
    }
    else {
        components[enemy.index] = 0;
    }
}

void EntityManager::collide() {
    std::vector<Entity> colliders;
    for (auto& collider : entities) {
        if (collider.hasComponent(this, Entity::Components::COLLISION)) {
            colliders.push_back(collider);
        }
    }
    std::vector<Entity> playerEntities;
    for (auto& collider : colliders) {
        if (collider.hasComponent(this, Entity::Components::PLAYER)) {
            playerEntities.push_back(collider);
        }
    }
    std::vector<Entity> enemies;
    for (auto& collider : colliders) {
        if (collider.hasComponent(this, Entity::Components::ENEMY)) {
            enemies.push_back(collider);
        }
    }
    std::vector<Entity> playerBullets;
    for (auto& collider : colliders) {
        if (collider.hasComponent(this, Entity::Components::PLAYER_BULLET)) {
            playerBullets.push_back(collider);
        }
    }
    
    for (int i = 0; i < playerEntities.size(); ++i) {
        Entity player = playerEntities[i];
        for (int j = 0; j < enemies.size(); ++j) {
        Entity enemy = enemies[j];
            
            SDL_Rect collider{ destinations[player.index]};
            SDL_Rect enemyCollider{ destinations[enemy.index]};
            SDL_Rect result;
            if (SDL_IntersectRect(&collider, &enemyCollider, &result)) {
                damageEntity(enemy);
            }
        }
    }

    for (int i = 0; i < playerBullets.size(); ++i) {
        Entity bullet = playerBullets[i];
        for (int j = 0; j < enemies.size(); ++j) {
            Entity enemy = enemies[j];
            
            SDL_Rect collider{ destinations[bullet.index]};
            SDL_Rect enemyCollider{ destinations[enemy.index]};
            SDL_Rect result;
            if (SDL_IntersectRect(&collider, &enemyCollider, &result)) {
                damageEntity(enemy);
                damageEntity(bullet);
            }
        }
    }
}

void EntityManager::updateInput(Vector2 vel) {
    std::vector<Entity> playerEntities;
    for (auto& e : entities) {
        if (e.hasComponent(this, Entity::Components::PLAYER)) {
            playerEntities.push_back(e);
        }
    }
    for (auto& player : playerEntities) {
        velocities[player.index] = vel;
    }
}

void EntityManager::updateShooting(float dt, SDL_Rect bulletSpriteSrc, int scale) {
    std::vector<Entity> playerEntities;
    for (auto& e : entities) {
        if (e.hasComponent(this, Entity::Components::PLAYER)) {
            playerEntities.push_back(e);
        }
    }
    
    for (auto& player : playerEntities) {
        static Uint64 tp = 0;
        if (tp < SDL_GetTicks64()) {
            tp = SDL_GetTicks64() + this->players[player.index].shootingCooldown;
            
            Entity e = getPooledEntity();
           
            Vector2 spawPoint = {positions[player.index].x + destinations[player.index].w,
                positions[player.index].y + destinations[player.index].h / 2};
        
            poolBullet(e, bulletSpriteSrc, spawPoint, scale);
            return;
        }
    }
}

void EntityManager::createPool() {
    createEntity();
}

Entity EntityManager::getPooledEntity() {
    for (auto& e : entities) {
        if (components[e.index] == 0) {
            return e;
        }
    }
    Entity e = createEntity();
    return e;
}

void EntityManager::poolPlayer(Entity e, SDL_Rect src, Vector2 pos, int scale) {
    SDL_Rect dst = {pos.x, pos.y, src.w * scale, src.h * scale};
    
    setPositionComponent(e, pos);
    setVelocityComponent(e, {0, 0} );
    setSourceComponent(e, src);
    setDestinationComponent(e, dst);
    setHealthComponent(e, 3);
    setCollisionComponent(e);
    setPlayerComponent(e, {100});
}

void EntityManager::poolEnemy(Entity e, SDL_Rect src, Vector2 pos, int scale) {
    SDL_Rect dst = {pos.x, pos.y, src.w * scale, src.h * scale};
    
    setPositionComponent(e, pos);
    setVelocityComponent(e, {-50, 0} );
    setSourceComponent(e, src);
    setDestinationComponent(e, dst);
    setHealthComponent(e, 1);
    setCollisionComponent(e);
    setEnemyComponent(e);
}

void EntityManager::poolBullet(Entity e, SDL_Rect src, Vector2 pos, int scale) {
    SDL_Rect dst = {pos.x, pos.y, src.w * scale, src.h * scale};
    
    setPositionComponent(e, pos);
    setVelocityComponent(e, {100, 0} );
    setSourceComponent(e, src);
    setDestinationComponent(e, dst);
    setHealthComponent(e, 1);
    setCollisionComponent(e);
    setPlayerBulletComponent(e);
}
