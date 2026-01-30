#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <iostream>
#include "renderer.h"
#include "simulation.h"


void print_stats(const Simulation* simulation) {
    float sq_sum = sqrtf(2.0f);
    int people_size = get_people_size(simulation);
    if (people_size == 0) return;

    float total_r = 0.0f;
    int radical_count = 0;

    for (int i = 0; i < people_size; i++) {
        float bx = get_person_belief_x(simulation, i);
        float by = get_person_belief_y(simulation, i);
        float r = sqrtf(bx * bx + by * by)/sq_sum;

        total_r += r;
        if (r > 0.8f) {
            radical_count++;
        }
    }

    float avg_radicalism = total_r / people_size;

    printf("Avg Radicalism: %.2f | Radicals: %d/%d\n",
           avg_radicalism, radical_count, people_size);
    fflush(stdout);
}


int main() {
    Simulation* simulation = init_simulation();
    
    Renderer renderer;
    renderer.init();

    int t = 0;
    bool isRunning = true;
    SDL_Event event;
    Uint32 pixels[MAP_WIDTH * MAP_HEIGHT];
    while (isRunning) {
        update_simulation(simulation,t);
        // 1. Handle Events 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // 2. Clear the screen
        SDL_SetRenderDrawColor(renderer.sdl_renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer.sdl_renderer);

        for(int y = 0;y < MAP_HEIGHT;y++){
            for(int x = 0;x < MAP_WIDTH;x++) {
                int person_id = get_person_id_from_pos(simulation, x, y);
                float belief_x = get_person_belief_x(simulation, person_id);
                float belief_y = get_person_belief_y(simulation, person_id);

                Uint8 r = (Uint8)((belief_x + 1.0f) * 127.5f);
                Uint8 b = (Uint8)((belief_y + 1.0f) * 127.5f);
                
                pixels[y * MAP_WIDTH + x] = (r << 24) | (0 << 16) | (b << 8) | 255;
            }
        }
        SDL_UpdateTexture(renderer.texture, NULL, pixels, MAP_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer.sdl_renderer, renderer.texture, NULL, NULL);
        SDL_RenderPresent(renderer.sdl_renderer);

        t++;
        if(t % 60 == 0) print_stats(simulation);
        SDL_Delay(16);
    }


    delete_simulation(simulation);
    return 0;
}

