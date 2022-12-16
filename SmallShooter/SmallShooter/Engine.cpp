#include "Engine.h"
#include <cstdio>
#include <SDL_image.h>

Engine::Engine(int w, int h) {
    initSDL(w, h);
}

bool Engine::initSDL(int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if(!initModules(w, h)) {
        printf( "Failed to create window!\n" );
    }
    return true;
}

bool Engine::initModules(int w, int h) {
    window = SDL_CreateWindow("TB2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    
    const int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }
    if(TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return  false;
    }
    return true;
}

bool Engine::loadTexture(std::string path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return false;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTexture == nullptr) {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        return false;
    }
    SDL_FreeSurface(loadedSurface);
    texture = newTexture;
    if(texture == nullptr) {
        printf( "Failed to load texture image!\n" );
        return false;
    }
    return true;
}

bool Engine::loadFont(const char* path, int size) {
    font = TTF_OpenFont( path, size);
    if(font == nullptr) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        return  false;
    }
    return true;
}

void Engine::drawTexture(SDL_Rect src, SDL_FRect dst) {
    SDL_RenderCopyF(renderer, texture, &src, &dst);
}

void Engine::drawTexture(SDL_Rect src, SDL_Rect dst) {
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void Engine::setText(const char* text) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, yellow);
    if(textSurface == nullptr)
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return;
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(textTexture == nullptr) {
        printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return;
    }
    w = textSurface->w;
    h = textSurface->h;
    SDL_FreeSurface(textSurface);
}

void Engine::drawText(int x, int y) {
    SDL_Rect dst{ x - w / 2, y - h / 2, w, h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
}

void Engine::render() {
    SDL_RenderClear(renderer);
}

void Engine::present() {
    SDL_RenderPresent(renderer);
}

void Engine::cleanup() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
    
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}
