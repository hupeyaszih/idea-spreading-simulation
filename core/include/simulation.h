#ifndef SIMULATION_H
#define SIMULATION_H

#ifdef __cplusplus
extern "C"{
#endif

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

#define FRIEND_COUNT 16 
#define REPULSION_THRESHOLD 0.85f
#define FRIEND_INFLUENCE_POWER 0.35f

typedef struct Simulation Simulation;
typedef struct Person Person;

Simulation* init_simulation();
void delete_simulation(Simulation* simulation);

Person* get_person(const Simulation* simulation,int person_id);
int get_person_id_from_pos(const Simulation* simulation, int pos_x, int pos_y);
float get_person_belief_x(const Simulation* simulation,int person_id);
float get_person_belief_y(const Simulation* simulation,int person_id);
float get_person_openness(const Simulation* simulation,int person_id);

int get_people_size(const Simulation* simulation);

void set_person_belief_x(const Simulation* simulation,int person_id,float belief_x);
void set_person_belief_y(const Simulation* simulation,int person_id,float belief_y);

void update_simulation(Simulation* simulation,int t);

#ifdef __cplusplus
}
#endif

#endif
