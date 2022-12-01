#include "EntityManager.h"

Entity* EntityManager::createEntity() {
    entities.emplace_back(entities.size());
	
    positions.push_back({0, 0});
    sources.push_back({0,0,0,0});
    return &entities[entities.size() - 1];
}

void EntityManager::setPositionField(Entity* e, Vector2 pos) {
    e->components |= Entity::Components::POSITION;
    positions[e->index] = pos;
}

void EntityManager::setSourceField(Entity* e, SDL_Rect src) {
    e->components |= Entity::Components::SCR;
    sources[e->index] = src;
}



