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
    for (auto e : entities) {
        if (e.hasComponent(Entity::Components::SCR)) {
            entitiesVector.push_back(&e);
        }
    }
    for (auto e : entities) {
        eng.drawTexture(sources[e.index],{positions[e.index].x, positions[e.index].y,
                    sources[e.index].w * scale, sources[e.index].h * scale});
    }
}

void EntityManager::updatePositions(float dt) {
    std::vector<Entity*> entitiesVector;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::POSITION) && e.hasComponent(Entity::Components::VELOCITY)) {
            entitiesVector.push_back(&e);
        }
    }
    for (auto e : entitiesVector) {
        updatePosition( positions[e->index], velocities[e->index], dt);
    }
}

void EntityManager::collide() {
    //todo 
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
                onCollision(*e, healths[e->index]);
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
    for (auto player : players) {
        velocities[player->index] = vel;
    }
}

void EntityManager::updateShooting(float dt) {
    std::vector<Entity*> players;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::PLAYER)) {
            players.push_back(&e);
        }
    }
    std::vector<Entity*> bullets;
    for (auto& e : entities) {
        if (e.hasComponent(Entity::Components::BULLET)) {
            bullets.push_back(&e);
        }
    }
    int i = 0;
    for (auto player : players) {
        while (healths[bullets[i]->index] <= 0) {
            i++;
            if (i > bullets.size()) {
                i = 0;
            }
        }
        positions[bullets[i]->index] = positions[player->index];
        healths[bullets[i]->index] = 0;

    }
}



