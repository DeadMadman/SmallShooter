#pragma once
#include <vector>

#include "Entity.h"

class EntityManager {
public:
    std::vector<Entity> entities{};
    std::vector<Vector2> positions{};
    std::vector<SDL_Rect> sources{};

    Entity* createEntity();
    void setPositionField(Entity* e, Vector2 pos);
    void setSourceField(Entity* e, SDL_Rect src);
    
};
