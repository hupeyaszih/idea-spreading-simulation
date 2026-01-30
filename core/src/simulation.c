#include "simulation.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct Simulation{
    Person* people;
    int peopleSize;

    float* belief_x_buffer;
    float* belief_y_buffer;
};

struct Person{
    int id;
    float belief_x; //
    float belief_y; //
    float openness; //
    
    int friends[FRIEND_COUNT];
};


static inline float clamp(float value, float min, float max) {
    if(value < min) return min;
    if(value > max) return max;
    return value;
}


static inline int get_person_id(int x,int y) {return y * MAP_WIDTH+ x;}
static inline int get_person_x(int person_id) {return person_id % MAP_WIDTH;}
static inline int get_person_y(int person_id) {return person_id / MAP_WIDTH;}
static inline int get_neighbour_person_id(int person_id, int dx, int dy) {
    int person_x = get_person_x(person_id)+dx;
    int person_y = get_person_y(person_id)+dy;

    if(person_x < 0 || person_x >= MAP_WIDTH) return -1;
    if(person_y < 0 || person_y >= MAP_HEIGHT) return -1;
    
    return get_person_id(person_x, person_y);
}

static inline float get_average_belief_x(const Simulation* simulation,int person_id) {
    float person_belief_x = get_person_belief_x(simulation, person_id);
    float total_belief_x = 0;
    float weight_sum = 0;
    for(int y = -1;y < 2;y++) {
        for(int x = -1;x < 2;x++) {
            if(x == 0 && y == 0) continue;
            int neighbour_person_id =  get_neighbour_person_id(person_id, x, y);
            if(neighbour_person_id == -1) continue;
            
            float neighbour_belief_x = get_person_belief_x(simulation, neighbour_person_id);

            float diff = fabs(neighbour_belief_x - person_belief_x);
            if(diff <= REPULSION_THRESHOLD) {
                total_belief_x += neighbour_belief_x;
            }else {
                float openness = get_person_openness(simulation, person_id);
                float k = (diff - REPULSION_THRESHOLD) / (1.0f - REPULSION_THRESHOLD); // 0..1
                float repulsion = person_belief_x + k * 0.3f * (person_belief_x - neighbour_belief_x)*openness;
                repulsion = clamp(repulsion, -1.0f, 1.0f);
                total_belief_x += repulsion;
            }
            weight_sum++;
        }
    }

    Person* p = get_person(simulation, person_id);
    for (int i = 0; i < FRIEND_COUNT; i++) {
        int fid = p->friends[i];
        float fb = get_person_belief_x(simulation, fid);

        float diff = fabs(fb - person_belief_x);

        // diff = 0  → weight = 1
        // diff = 1  → weight = 0
        float w = 1.0f - diff;
        w = clamp(w, 0.0f, 1.0f);

        // friend influence
        total_belief_x += fb * w * FRIEND_INFLUENCE_POWER;
        weight_sum += w * FRIEND_INFLUENCE_POWER;

    }

    if(weight_sum <= 0) return person_belief_x;
    return total_belief_x/weight_sum;
}
static inline float get_average_belief_y(const Simulation* simulation,int person_id) {
    float person_belief_y = get_person_belief_y(simulation, person_id);
    float total_belief_y = 0;
    float weight_sum = 0;
    for(int y = -1;y < 2;y++) {
        for(int x = -1;x < 2;x++) {
            if(x == 0 && y == 0) continue;
            int neighbour_person_id =  get_neighbour_person_id(person_id, x, y);
            if(neighbour_person_id == -1) continue;

            float neighbour_belief_y = get_person_belief_y(simulation, neighbour_person_id);

            float diff = fabs(neighbour_belief_y - person_belief_y);
            if(diff <= REPULSION_THRESHOLD){
                total_belief_y += neighbour_belief_y; 
            }else {
                float openness = get_person_openness(simulation, person_id);
                float k = (diff - REPULSION_THRESHOLD) / (1.0f - REPULSION_THRESHOLD); // 0..1
                float repulsion = person_belief_y + k * 0.3f * (person_belief_y - neighbour_belief_y) * openness;
                repulsion = clamp(repulsion, -1.0f, 1.0f);
                total_belief_y += repulsion;
            } 
            weight_sum++;
        }
    }
    Person* p = get_person(simulation, person_id);
    for (int i = 0; i < FRIEND_COUNT; i++) {
        int fid = p->friends[i];
        float fb = get_person_belief_y(simulation, fid);

        float diff = fabs(fb - person_belief_y);

        // diff = 0  → weight = 1
        // diff = 1  → weight = 0
        float w = 1.0f - diff;
        w = clamp(w, 0.0f, 1.0f);

        // friend influence
        total_belief_y += fb * w * FRIEND_INFLUENCE_POWER;
        weight_sum += w * FRIEND_INFLUENCE_POWER;

    }
    if(weight_sum <= 0) return person_belief_y;
    return total_belief_y/weight_sum;
}

Simulation* init_simulation() {
    srand(time(NULL));
    Simulation* simulation;
    simulation = malloc(sizeof(Simulation));
    simulation->peopleSize = MAP_WIDTH*MAP_HEIGHT;
    simulation->people = malloc(sizeof(Person)*simulation->peopleSize);
    simulation->belief_x_buffer = malloc(sizeof(float)*simulation->peopleSize);
    simulation->belief_y_buffer = malloc(sizeof(float)*simulation->peopleSize);

    for(int y = 0;y < MAP_HEIGHT;y++) {
        for(int x = 0;x < MAP_WIDTH;x++){
            int id = get_person_id(x, y);
            Person person;
            person.id = id;

            person.belief_x = ((float)rand()/RAND_MAX)*2-1;
            person.belief_y = ((float)rand()/RAND_MAX)*2-1;

            person.openness = (float)rand()/RAND_MAX;

            for(int f = 0;f < FRIEND_COUNT;f++) {
                person.friends[f] = rand()%(simulation->peopleSize);
                while(person.friends[f] == id) {
                    person.friends[f] = rand()%(simulation->peopleSize);
                }
            }

            simulation->people[id] = person;
        }
    }

    return simulation;
}
void delete_simulation(Simulation *simulation){
    free(simulation->people);
    simulation->people = NULL;
    
    free(simulation->belief_x_buffer);
    simulation->belief_x_buffer = NULL;

    free(simulation->belief_y_buffer);
    simulation->belief_y_buffer = NULL;

    free(simulation);
    simulation = NULL;
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

            float r = sqrtf(belief_x*belief_x + belief_y*belief_y);
            float openness_eff = openness * (1.0f - r*r); // quadratic attenuation
            if(openness_eff < 0) openness_eff = 0;
            if(openness_eff > 1) openness_eff = 1;
            float new_belief_x = belief_x + openness_eff * (average_belief_x - belief_x);
            float new_belief_y = belief_y + openness_eff * (average_belief_y - belief_y);


            float noise_x = ((float)rand()/RAND_MAX)*2 - 1;
            float noise_y = ((float)rand()/RAND_MAX)*2 - 1;
            new_belief_x += noise_x * openness * 0.03f; // %1'lik bir değişim
            new_belief_y += noise_y * openness * 0.03f;

            new_belief_x = clamp(new_belief_x, -1.0f, 1.0f);
            new_belief_y = clamp(new_belief_y, -1.0f, 1.0f);

            simulation->belief_x_buffer[person_id] = new_belief_x;
            simulation->belief_y_buffer[person_id] = new_belief_y;
        }
    }

    for(int y = 0;y < MAP_HEIGHT;y++) {
        for(int x = 0;x < MAP_WIDTH;x++) {
            int person_id = get_person_id(x, y);
            set_person_belief_x(simulation, person_id, simulation->belief_x_buffer[person_id]);
            set_person_belief_y(simulation, person_id, simulation->belief_y_buffer[person_id]);
        }
    }
     
}


Person* get_person(const Simulation* simulation,int person_id) {
    if(person_id < 0 || person_id >= simulation->peopleSize) return NULL;
    return &simulation->people[person_id];
}
int get_person_id_from_pos(const Simulation *simulation, int pos_x, int pos_y) {
    return get_person_id(pos_x, pos_y);
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

int get_people_size(const Simulation *simulation) {
    return simulation->peopleSize;
}

////SETTERS
void set_person_belief_x(const Simulation *simulation, int person_id, float belief_x){
    get_person(simulation, person_id)->belief_x = belief_x;
}
void set_person_belief_y(const Simulation *simulation, int person_id, float belief_y){
    get_person(simulation, person_id)->belief_y = belief_y;
}
