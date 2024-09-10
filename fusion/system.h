//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for system, an abstract space to accommodate isolations and provide basic rules and parameters
//

#ifndef FUSION_SYSTEM_H
#define FUSION_SYSTEM_H


#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include "unit_population.h"
#include "isolation.h"

class System {
private:
    double baseBirthRate;                                   // Base birth rate for populations
    double baseDeathRate;                                   // Base death rate for populations
    std::vector<std::shared_ptr<Isolation>> isolations;     // List of isolations in the system
    std::vector<std::vector<double>> barrierThresholds;     // Matrix to store barrier thresholds between isolations
    int nextPopulationId;                                   // Counter to generate unique population IDs

public:
    // Constructor to initialize the system with a set number of isolations and barrier thresholds
    System(int numIsolations, double baseBirth, double baseDeath)
            : baseBirthRate(baseBirth), baseDeathRate(baseDeath), nextPopulationId(0) {

        // Initialize isolations with some default resources (assuming 3 niche dimensions for simplicity)
        for (int i = 0; i < numIsolations; ++i) {
            std::vector<double> initialResources = {10.0, 10.0, 10.0};  // Example initial resource availability
            isolations.push_back(std::make_shared<Isolation>(initialResources));
        }

        // Initialize the barrier thresholds matrix (default barrier = 1.0 for all pairs)
        barrierThresholds.resize(numIsolations, std::vector<double>(numIsolations, 1.0));

        // Spawn one unit population in the first isolation
        spawnInitialPopulation();
    }

    // Destructor
    ~System() {
        isolations.clear();
        std::cout << "System destroyed, all isolations and populations cleared.\n";
    }

    // Getters
    [[nodiscard]] double getBaseBirthRate() const {
        return baseBirthRate;
    }

    [[nodiscard]] double getBaseDeathRate() const {
        return baseDeathRate;
    }

    // Get all isolations
    [[nodiscard]] const std::vector<std::shared_ptr<Isolation>>& getAllIsolations() const {
        return isolations;
    }

    // Get an isolation by index
    [[nodiscard]] std::shared_ptr<Isolation> getIsolation(int index) const {
        if (index >= 0 && index < isolations.size()) {
            return isolations[index];
        } else {
            throw std::out_of_range("Isolation index is out of bounds");
        }
    }

    [[nodiscard]] size_t getNumberOfIsolations() const {
        return isolations.size();
    }

    [[nodiscard]] const std::vector<std::vector<double>>& getBarrierThresholds() const {
        return barrierThresholds;
    }

    [[nodiscard]] int getNextPopulationId() const {
        return nextPopulationId;
    }

    // Setters
    void setBaseBirthRate(double birthRate) {
        baseBirthRate = birthRate;
    }

    void setBaseDeathRate(double deathRate) {
        baseDeathRate = deathRate;
    }

    // Method to spawn the initial unit population in the first isolation
    void spawnInitialPopulation() {
        std::vector<double> resourceUse = {1.0, 1.0, 1.0};  // Default resource usage for all dimensions
        double mutationRate = 0.01;
        double mobility = 0.1;
        double reproductivity = 0.5;

        UnitPopulation initialPopulation(nextPopulationId++, 0, std::nullopt, mutationRate, mobility, resourceUse, reproductivity);
        isolations[0]->addUnitPopulation(initialPopulation);

        std::cout << "Initial population spawned in the first isolation.\n";
    }

    // Method to set barrier thresholds between isolations
    void setBarrierThreshold(int isolationA, int isolationB, double threshold) {
        if (isolationA >= 0 && isolationA < isolations.size() && isolationB >= 0 && isolationB < isolations.size()) {
            barrierThresholds[isolationA][isolationB] = threshold;
            barrierThresholds[isolationB][isolationA] = threshold;  // Symmetric barrier between A and B
            std::cout << "Barrier threshold set between Isolation " << isolationA
                      << " and Isolation " << isolationB << ": " << threshold << "\n";
        } else {
            std::cerr << "Invalid isolation indices provided.\n";
        }
    }
};


#endif //FUSION_SYSTEM_H
