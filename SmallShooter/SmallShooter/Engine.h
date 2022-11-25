#pragma once
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Engine {
public:
    Engine(int w, int h);
    
    bool initSDL(int w, int h);
    void createWindow(int w, int h);
    void loadSurface();
    void loadMedia();

    void update();
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_Surface* screenSurface = nullptr;

    SDL_Surface* imgSurface = nullptr;

    SDL_Rect src = {72, 114, 45, 45};
    SDL_Rect dst = {100, 100, 300, 300};
};
