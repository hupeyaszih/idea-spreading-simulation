#include "simulation.h"
#include <stdio.h>
int main() {
    Simulation* simulation = init_simulation(); 

    for(int i = 0;i < 1000;i++) {
        update_simulation(simulation, i);
        printf("%f\n",get_person_belief_x(simulation, 10));
    }
    return 0;
}





////
///TODO:SDL visualization layer
///TODO:Camera + zoom
///TODO:color mapping
///dear programmer ,who is reading this comment line, this code was known only by God and me. Now, only god knows.
///
///     AUTHOR:
///---HUPEYASZIH---
////
