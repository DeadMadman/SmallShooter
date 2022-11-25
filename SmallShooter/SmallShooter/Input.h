#pragma once
#include <array>
#include <SDL_scancode.h>

class Input {
public:
    struct KeyMappings {
        SDL_Scancode moveLeft = SDL_SCANCODE_A;
        SDL_Scancode moveRight = SDL_SCANCODE_D;
        SDL_Scancode moveUp = SDL_SCANCODE_W;
        SDL_Scancode moveDown = SDL_SCANCODE_S;
    };

    std::array<int, SDL_NUM_SCANCODES> prevStates;
    std::array<int, SDL_NUM_SCANCODES> currStates;

    bool isDown(SDL_Scancode scancode);
    bool isUp(SDL_Scancode scancode);
    bool isPressed(SDL_Scancode scancode);
    bool isReleased(SDL_Scancode scancode);

    void updateKey(SDL_Scancode scancode, bool isDown);
    void refresh();
    void update(float dt);

    KeyMappings keys{};
    float moveSpeed{3.f};
    float lookSpeed{1.5f};
    
};
