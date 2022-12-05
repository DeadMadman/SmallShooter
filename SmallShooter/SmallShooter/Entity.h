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
        VELOCITY = 1 << 2,
        COLLISION = 1 << 3,
        HEALTH = 1 << 4,
        PLAYER = 1 << 5,
        ENEMY = 1 << 6,
        BULLET = 1 << 7,
        SIZE = 1 << 8
    };
    std::bitset<SIZE> components;

    bool hasComponent(Components c);
};

static std::bitset<Entity::Components::SIZE> asBitset(Entity::Components c) {
    return {c};
}

inline void updatePosition(Vector2& pos, Vector2 vel, float dt) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
}

inline void onCollision(Entity& e, int& hp) {
    if (e.hasComponent(Entity::HEALTH)) {
        hp--;
        if (hp == 0) {
            //todo kill
        }
    }
}
