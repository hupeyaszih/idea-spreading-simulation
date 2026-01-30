# idea-simulation
A 2D simulation of idea spreading among individuals, including social influence, neighbor interactions, and visualization using SDL2.


# Idea Spreading Simulation

A 2D simulation that models how ideas and beliefs spread among a population. Each person in the simulation has beliefs represented in 2D space, an openness value, and a set of friends who influence them. Belief updates consider neighbors, friends, repulsion from extreme differences, and a small random noise.

![Simulation Screenshot](screenshot.png) <!-- Eğer bir ekran görüntüsü eklemek istersen -->

## Features

- 2D grid of people with beliefs in X and Y dimensions.
- Influence from local neighbors and a small set of random friends.
- Repulsion when beliefs differ too much (modeling polarization).
- Random noise to simulate unpredictable changes.
- Real-time 2D rendering using SDL2.
- Adjustable parameters like repulsion threshold, friend influence, and noise level.

## Getting Started

### Requirements

- C compiler (GCC / Clang)
- CMake 3.20+
- SDL2 library installed

### Build

```bash
git clone https://github.com/hupeyaszih/idea-spreading-simulation.git
cd idea-spreading-simulation
mkdir build
cd build
cmake build ..
cmake --build .
```
### NOTE
Default parameters are defined in simulation.h:

MAP_WIDTH / MAP_HEIGHT – Size of the 2D grid.

FRIEND_COUNT – Number of random friends each person has.

REPULSION_THRESHOLD – Maximum difference tolerated before repulsion occurs.

FRIEND_INFLUENCE_POWER – How much friends influence beliefs.

Feel free to tweak these values and recompile to explore different behaviors.
