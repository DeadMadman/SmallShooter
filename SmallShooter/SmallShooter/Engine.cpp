#include "Engine.h"

#include <cstdio>

Engine::Engine(int w, int h) {
    if (initSDL(w, h)) {
        
       
    }
}

bool Engine::initSDL(int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    createWindow(w, h);
    loadMedia();
    return true;
}

void Engine::createWindow(int w, int h) {
    window = SDL_CreateWindow("TB2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else
    {
        loadSurface();
    }
}

void Engine::loadSurface() {
    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
}

void Engine::loadMedia() {
    imgSurface = SDL_LoadBMP("img.bmp");
    if( imgSurface == NULL ) {
        printf( "Unable to load image %s! SDL Error: %s\n", ".bmp", SDL_GetError() );
    }
}

void Engine::update() {
    SDL_BlitSurface( imgSurface, &src, screenSurface, &dst );
    SDL_UpdateWindowSurface(window);
}

void Engine::cleanup() {
    SDL_FreeSurface(imgSurface);
    SDL_FreeSurface(screenSurface);
    SDL_DestroyWindow(window);

    imgSurface = nullptr;
    screenSurface = nullptr;
    window = nullptr;
}
