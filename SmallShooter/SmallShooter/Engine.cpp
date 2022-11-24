#include "Engine.h"

#include <cstdio>

Engine::Engine() {
    initSDL();
}

void Engine::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else
    {
        createWindow();
    }
}

void Engine::createWindow() {
    window = SDL_CreateWindow("TB2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else
    {
        createSurface();
    }
}

void Engine::createSurface() {
    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
}

void Engine::update() {
    
}

void Engine::cleanup() {
    SDL_FreeSurface(screenSurface);
    SDL_DestroyWindow(window);

    screenSurface = nullptr;
    window = nullptr;
}
