#include "simulation.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct Simulation{
    Person* people;
    int peopleSize;
};

struct Person{
    int id;
    float belief_x; //
    float belief_y; //
    float openness; //
};


static inline float clamp(float value, float min, float max) {
    if(value < min) return min;
    if(value > max) return max;
    return value;
}


static inline int get_person_id(int x,int y) {return y * MAP_WIDTH+ x;}
static inline int get_person_x(int person_id) {person_id % MAP_WIDTH;}
static inline int get_person_y(int person_id) {return (person_id-get_person_x(person_id))/MAP_WIDTH;}
static inline int get_neighbour_person_id(int person_id, int dx, int dy) {
    int person_x = get_person_x(person_id)+dx;
    int person_y = get_person_y(person_id)+dy;

    if(person_x < 0 || person_x >= MAP_WIDTH) return -1;
    if(person_y < 0 || person_y >= MAP_HEIGHT) return -1;
    
    return get_person_id(person_x, person_y);
}

static inline float get_average_belief_x(const Simulation* simulation,int person_id) {
    int from_x = get_person_x(person_id);
    int from_y = get_person_y(person_id);

    float total_belief_x = 0;
    int total_neighbour_count = 0;
    for(int y = -1;y < 2;y++) {
        for(int x = -1;x < 2;x++) {
            int position_x = from_x + x;
            int position_y = from_y + y;

            int neighbour_person_id =  get_neighbour_person_id(person_id, position_x, position_y);
            if(neighbour_person_id == -1) continue;
            
            total_belief_x += get_person_belief_x(simulation, neighbour_person_id);
            total_neighbour_count++;
        }
    }
    return total_belief_x/total_neighbour_count;
}
static inline float get_average_belief_y(const Simulation* simulation,int person_id) {
    int from_x = get_person_x(person_id);
    int from_y = get_person_y(person_id);

    float total_belief_y = 0;
    int total_neighbour_count = 0;
    for(int y = -1;y < 2;y++) {
        for(int x = -1;x < 2;x++) {
            int position_x = from_x + x;
            int position_y = from_y + y;

            int neighbour_person_id =  get_neighbour_person_id(person_id, position_x, position_y);
            if(neighbour_person_id == -1) continue;

            total_belief_y += get_person_belief_y(simulation, neighbour_person_id);
            total_neighbour_count++;
        }
    }
    return total_belief_y/total_neighbour_count;
}

Simulation* init_simulation() {
    Simulation* simulation;
    simulation = malloc(sizeof(Simulation));
    simulation->peopleSize = MAP_WIDTH*MAP_HEIGHT;
    simulation->people = malloc(sizeof(Person)*simulation->peopleSize);
     
    for(int y = 0;y < MAP_HEIGHT;y++) {
        for(int x = 0;x < MAP_WIDTH;x++){
            int id = get_person_id(x, y);
            Person person;
            person.id = id;

            person.belief_x = ((float)rand()/RAND_MAX)*2-1;
            person.belief_y = ((float)rand()/RAND_MAX)*2-1;

            person.openness = (float)rand()/RAND_MAX;
        
            simulation->people[id] = person;
        }
    }

    return simulation;
}


void update_simulation(Simulation *simulation,int t) {
    for(int y = 0;y < MAP_HEIGHT;y++){
        for(int x = 0;x < MAP_WIDTH;x++){
            int person_id = get_person_id(x, y);
            float average_belief_x = get_average_belief_x(simulation, person_id);
            float average_belief_y = get_average_belief_y(simulation, person_id);

            float belief_x = get_person_belief_x(simulation, person_id);
            float belief_y = get_person_belief_y(simulation, person_id);
            float openness = get_person_openness(simulation, person_id);

            float r = sqrtf(belief_x*belief_x+belief_y*belief_y);
            float openness_eff = openness * (1.0f - r);
            if(openness_eff < 0) openness_eff = 0;
            
            float new_belief_x = openness_eff * (average_belief_x - belief_x);
            float new_belief_y = openness_eff * (average_belief_y - belief_y);

            new_belief_x = clamp(new_belief_x, -1.0f, 1.0f);
            new_belief_y = clamp(new_belief_y, -1.0f, 1.0f);

            set_person_belief_x(simulation, person_id, new_belief_x); 
            set_person_belief_y(simulation, person_id, new_belief_y);
        }
    }
     
}


Person* get_person(const Simulation* simulation,int person_id) {
    if(person_id < 0 || person_id > simulation->peopleSize) return NULL;
    return &simulation->people[person_id];
}

////GETTERS
float get_person_belief_x(const Simulation* simulation,int person_id){
    return get_person(simulation, person_id)->belief_x;
}

float get_person_belief_y(const Simulation* simulation,int person_id){
    return get_person(simulation, person_id)->belief_y;
}
float get_person_openness(const Simulation *simulation, int person_id){
    return get_person(simulation, person_id)->openness;
}

////SETTERS
void set_person_belief_x(const Simulation *simulation, int person_id, float belief_x){
    get_person(simulation, person_id)->belief_x = belief_x;
}
void set_person_belief_y(const Simulation *simulation, int person_id, float belief_y){
    get_person(simulation, person_id)->belief_x = belief_y;
}
