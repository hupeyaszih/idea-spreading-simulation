#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <iostream>
#include "renderer.h"
#include "simulation.h"

int main() {
    Simulation* simulation = init_simulation();
    
    Renderer renderer;
    renderer.init();

    int t = 0;
    bool isRunning = true;
    SDL_Event event;
    while (isRunning) {
        update_simulation(simulation,t);
        // 1. Handle Events (This makes the window appear!)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // 2. Clear the screen
        SDL_SetRenderDrawColor(renderer.sdl_renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer.sdl_renderer);

        // 3. Draw stuff here later...
        for(int y = 0;y < MAP_HEIGHT;y++){
            for(int x = 0;x < MAP_WIDTH;x++) {
                int person_id = get_person_id_from_pos(simulation, x, y);
                float belief_x = get_person_belief_x(simulation, person_id);
                float belief_y = get_person_belief_y(simulation, person_id);

                float r = (belief_x+1)*127.5;
                float b = (belief_y+1)*127.5;

                SDL_SetRenderDrawColor(renderer.sdl_renderer, (Uint8)r, 0, (Uint8)b, 255);

                float scale_factor_x = (float)SCREEN_WIDTH/MAP_WIDTH;
                float scale_factor_y = (float)SCREEN_HEIGHT/MAP_HEIGHT;
                float scale = (scale_factor_x < scale_factor_y) ? scale_factor_x : scale_factor_y;

                SDL_Rect rect;
                rect.x = (int)x*scale;
                rect.y = (int)y*scale;
                rect.w = (int)scale;
                rect.h = (int)scale;

                SDL_RenderFillRect(renderer.sdl_renderer, &rect);
            }
        }
        

        // 4. Show the result
        SDL_RenderPresent(renderer.sdl_renderer);
        t++;
        SDL_Delay(16);
    }


    delete_simulation(simulation);
    return 0;
}
