#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Engine {
public:
    Engine(int w, int h);
    
    bool loadTexture(std::string path);
    bool loadFont(const char* path, int size);
    void drawTexture(SDL_Rect src, SDL_FRect dst);
    void drawTexture(SDL_Rect src, SDL_Rect dst);

    void setText(const char* text);
    void drawText(int x, int y);
    
    void render();
    void present();
    void cleanup();
private:
    
    bool initSDL(int w, int h);
    bool initModules(int w, int h);
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    SDL_Texture* texture = nullptr;
    SDL_Texture* textTexture = nullptr;
    TTF_Font* font = nullptr;

    int w, h;

    SDL_Color black{ 129, 151, 150, 255 };
    SDL_Color blue{ 164, 221, 216, 255 };
    SDL_Color yellow{ 232, 193, 112, 255 };

};
