#include "Entity.h"

bool Entity::hasComponent(Components c) {
    return (components & asBitset(c)) != 0;
}