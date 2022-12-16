#include "Entity.h"
#include "EntityManager.h"
bool Entity::hasComponent(EntityManager* manager, Components c) {
    return (manager->components[this->index] & asBitset(c)) != 0;
}