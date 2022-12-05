#include "Input.h"

#include <iostream>

bool Input::isDown(SDL_Scancode scancode) {
    return currStates[scancode];
}

bool Input::isUp(SDL_Scancode scancode) {
    return !currStates[scancode];
}

bool Input::isPressed(SDL_Scancode scancode) {
    return currStates[scancode] && !prevStates[scancode];
}

bool Input::isReleased(SDL_Scancode scancode) {
    return !currStates[scancode] && prevStates[scancode];
}

void Input::updateKey(SDL_Scancode scancode, bool isDown) {
    currStates[scancode] = isDown;
}

void Input::refresh() {
    memcpy(prevStates.data(), currStates.data(), sizeof(currStates));
}

void Input::update(float dt) {}
