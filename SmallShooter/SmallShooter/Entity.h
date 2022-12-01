#pragma once
#include <bitset>
#include <SDL.h>

struct Vector2
{
    float x = 0;
    float y = 0;
};

class Entity {
public:
    Entity(int i) { index = i; }
    
    int index;

    enum Components : unsigned long long {
        POSITION = 1,
        SCR = 1 << 1,
        SIZE = 1 << 2
    };
    std::bitset<SIZE> components;

    bool hasComponent(Components c);
};

static std::bitset<Entity::Components::SIZE> asBitset(Entity::Components c) {
    return {c};
}

inline void updatePosition(Vector2& pos) {
    pos.x++;
}


