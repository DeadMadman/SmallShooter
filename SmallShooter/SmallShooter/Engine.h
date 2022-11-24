#pragma once
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Engine {
public:
    Engine();
    
    void initSDL();
    void createWindow();
    void createSurface();

    void update();
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_Surface* screenSurface = nullptr;
};
