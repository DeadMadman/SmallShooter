#include "EntityManager.h"

Entity* EntityManager::createEntity() {
    entities.emplace_back(entities.size());
    
    healths.push_back(0);
    positions.push_back({0, 0});
    velocities.push_back({0, 0});
    sources.push_back({0,0,0,0});
    
    return &entities[entities.size() - 1];
}

void EntityManager::setHealthComponent(Entity* e, int hp) {
    e->components |= Entity::Components::HEALTH;
    healths[e->index] = hp;
}

void EntityManager::setPositionComponent(Entity* e, Vector2 pos) {
    e->components |= Entity::Components::POSITION;
    positions[e->index] = pos;
}

void EntityManager::setVelocityComponent(Entity* e, Vector2 vel) {
    e->components |= Entity::Components::VELOCITY;
    velocities[e->index] = vel;
}

void EntityManager::setSourceComponent(Entity* e, SDL_Rect src) {
    e->components |= Entity::Components::SCR;
    sources[e->index] = src;
}

void EntityManager::setCollisionComponent(Entity* e) {
    e->components |= Entity::Components::COLLISION;
}

void EntityManager::setPlayerComponent(Entity* e) {
    e->components |= Entity::Components::PLAYER;
}

void EntityManager::setEnemyComponent(Entity* e) {
    e->components |= Entity::Components::ENEMY;
}

void EntityManager::setBulletComponent(Entity* e) {
    e->components |= Entity::Components::BULLET;
}

void EntityManager::renderEntities(Engine& eng, float scale) {
    std::vector<Entity*> entitiesVector;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::SCR)) {
            entitiesVector.push_back(&e);
        }
    }
    for (auto& e : entitiesVector) {
        eng.drawTexture(sources[e->index],{positions[e->index].x, positions[e->index].y,
                    sources[e->index].w * scale, sources[e->index].h * scale});
    }
}

void EntityManager::updatePositions(float dt, SDL_FRect bounds) {
    std::vector<Entity*> entitiesVector;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::POSITION) && e.hasComponent(Entity::Components::VELOCITY)) {
            entitiesVector.push_back(&e);
        }
    }
    for (auto& e : entitiesVector) {
        Vector2 pos = positions[e->index];
        if (pos.x < bounds.x || pos.x > bounds.w || pos.y < bounds.y || pos.y > bounds.h) {
            e->components = 0;
        }
        else {
            updatePosition( positions[e->index], velocities[e->index], dt);
        }
    }
}

void EntityManager::collide() {
    std::vector<Entity*> colliders;
    for (auto& collider : entities) {
        if (collider.hasComponent(Entity::Components::COLLISION)) {
            colliders.push_back(&collider);
        }
    }
    std::vector<Entity*> players;
    for (auto& collider : colliders) {
        if (collider->hasComponent(Entity::Components::PLAYER)) {
            players.push_back(collider);
        }
    }
    std::vector<Entity*> enemies;
    for (auto& collider : colliders) {
        if (collider->hasComponent(Entity::Components::ENEMY)) {
            enemies.push_back(collider);
        }
    }
    for (int i = 0; i < players.size(); ++i) {
        Entity* e = players[i];
        for (int j = 0; j < enemies.size(); ++j) {
        Entity* other = enemies[j];
            
            SDL_FRect collider{ positions[e->index].x, positions[e->index].y,
                static_cast<float>(sources[e->index].w), static_cast<float>(sources[e->index].h)};
            SDL_FRect otherCollider{ positions[other->index].x, positions[other->index].y,
                static_cast<float>(sources[other->index].w), static_cast<float>(sources[other->index].h)};
            SDL_FRect result;
            if (SDL_IntersectFRect(&collider, &otherCollider, &result)) {
                //onCollision(*e, healths[e->index]);
                onCollision(*other, healths[other->index]);
            }
        }
    }
}

void EntityManager::updateInput(Vector2 vel) {
    std::vector<Entity*> players;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::PLAYER)) {
            players.push_back(&e);
        }
    }
    for (auto& player : players) {
        velocities[player->index] = vel;
    }
}

void EntityManager::updateShooting(float dt, SDL_Rect bulletSpriteSrc) {
    std::vector<Entity*> players;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::PLAYER)) {
            players.push_back(&e);
        }
    }
    
    for (auto& player : players) {
       
        //todo add time for cooldown
        Entity* e = getPooledEntity();
        Vector2 spawPoint = {positions[player->index].x + sources[player->index].w,
            positions[player->index].y + sources[player->index].h / 2.0f};
        
        poolBullet(e, bulletSpriteSrc, spawPoint);
        return;
    }
}

void EntityManager::createPool() {
    createEntity();
}

Entity* EntityManager::getPooledEntity() {
    for (auto& e : entities) {
        if (e.components == 0) {
            return &e;
        }
    }
    Entity* e = createEntity();
    return e;
}

void EntityManager::poolPlayer(Entity* e, SDL_Rect playerSpriteSrc, int height) {
    setPositionComponent(e, {10.0f, height * 0.5f + 8});
    setVelocityComponent(e, {0.0f, 0.0f} );
    setSourceComponent(e, playerSpriteSrc);
    setHealthComponent(e, 3);
    setCollisionComponent(e);
    setPlayerComponent(e);
}

void EntityManager::createEnemy(Entity* e, SDL_Rect enemySpriteSrc, int width, int i) {
    setPositionComponent(e, {static_cast<float>(width), i * 600.0f / 20.0f});
    setVelocityComponent(e, {-100.0f, 0.0f} );
    setSourceComponent(e, enemySpriteSrc);
    setHealthComponent(e, 1);
    setCollisionComponent(e);
    setEnemyComponent(e);
}

void EntityManager::poolBullet(Entity* e, SDL_Rect bulletSpriteSrc, Vector2 pos) {
    setPositionComponent(e, pos);
    setVelocityComponent(e, {100.0f, 0.0f} );
    setSourceComponent(e, bulletSpriteSrc);
    setHealthComponent(e, 1);
    setCollisionComponent(e);
    setBulletComponent(e);
}
