//
// Created by Tianjian on 9/8/2024.
// Program main loop
//

#include "director.h"

int main() {
    // Initialize Director with 3 isolations, birth rate = 0.5, death rate = 0.2
    Director director(3, 0.5, 0.2);

    // Run the simulation for a total time of 100 units
    director.runSimulation(100.0);

    return 0;
}
