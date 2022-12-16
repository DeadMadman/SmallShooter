#pragma once
#include <bitset>
#include <SDL.h>
#include <math.h>

struct Vector2
{
    int x = 0;
    int y = 0;
};

struct Player {
    Uint64 shootingCooldown = 1000;
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
        PLAYER_BULLET = 1 << 7,
        ENEMY_BULLET = 1 << 8,
        DST = 1 << 9,
        SIZE = 1 << 10
    };
    bool hasComponent(class EntityManager* manager, Components c);
};

static std::bitset<Entity::Components::SIZE> asBitset(Entity::Components c) {
    return {c};
}

inline void updatePosition(Vector2& pos, Vector2 vel, float dt) {
    pos.x += lroundf(static_cast<float>(vel.x) * dt);
    pos.y += lroundf( static_cast<float>(vel.y) * dt);
}
