#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

class Renderer{
    public:
        Renderer();
        ~Renderer();
        void init();

        SDL_Window* sdl_window;
        SDL_Renderer* sdl_renderer;
};

#endif
