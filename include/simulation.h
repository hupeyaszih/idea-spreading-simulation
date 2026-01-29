#ifndef SIMULATION_H
#define SIMULATION_H

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

typedef struct Simulation Simulation;
typedef struct Person Person;

Simulation* init_simulation();

Person* get_person(const Simulation* simulation,int person_id);
float get_person_belief_x(const Simulation* simulation,int person_id);
float get_person_belief_y(const Simulation* simulation,int person_id);
float get_person_openness(const Simulation* simulation,int person_id);

void set_person_belief_x(const Simulation* simulation,int person_id,float belief_x);
void set_person_belief_y(const Simulation* simulation,int person_id,float belief_y);

void update_simulation(Simulation* simulation,int t);

#endif
