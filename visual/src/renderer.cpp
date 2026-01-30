#include "renderer.h"
#include "simulation.h"
#include <iostream>

Renderer::Renderer() : sdl_window(nullptr), sdl_renderer(nullptr) {
}
Renderer::~Renderer() {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::init() {
    std::cout << "Initialization started..." << std::endl;
    int renderer_flags, window_flags;
    renderer_flags = SDL_RENDERER_ACCELERATED;
    window_flags = SDL_WINDOW_SHOWN;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL INIT ERROR" << std::endl;
        exit(1);
    }

    sdl_window = SDL_CreateWindow("Idea Simulation by HupeyasziH", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if(!sdl_window) {
        std::cout << "Failed to open window" << std::endl;
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, renderer_flags);

    if(!sdl_renderer) {
        std::cout << "Failed to create renderer" << std::endl;
        exit(1);
    }
    std::cout << "Initialization finished..." << std::endl;

    texture = SDL_CreateTexture(
        sdl_renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        MAP_WIDTH, MAP_HEIGHT
    );

}
