//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for unit population, an abstract intermediate species object
//


#ifndef FUSION_UNIT_POPULATION_H
#define FUSION_UNIT_POPULATION_H


#include <vector>
#include <string>
#include <optional>
#include <iostream>

class UnitPopulation {
private:
    int unitPopulationId;                          // Unique identifier for the unit population
    int locationId;                                // Island id where this population resides
    std::optional<int> parentUnitId;               // Optional parent unit id (could be none)
    double intrinsicMutationRate;                  // Mutation rate specific to this population
    double mobility;                               // Mobility rate of this population
    std::vector<double> resourceUsePerNiche;       // List of per capita resource use for all niche dimensions
    double reproductivity;                         // Reproductivity rate

public:
    // Constructor
    UnitPopulation(int id, int locId, std::optional<int> parentId, double mutationRate, double mobilityRate,
                   const std::vector<double>& resources, double reproductionRate)
            : unitPopulationId(id), locationId(locId), parentUnitId(parentId), intrinsicMutationRate(mutationRate),
              mobility(mobilityRate), resourceUsePerNiche(resources), reproductivity(reproductionRate) {}

    // Destructor
    ~UnitPopulation() = default;

    // Getters
    [[nodiscard]] int getId() const { return unitPopulationId; }
    [[nodiscard]] int getLocationId() const { return locationId; }
    [[nodiscard]] std::optional<int> getParentUnitId() const { return parentUnitId; }
    [[nodiscard]] double getMutationRate() const { return intrinsicMutationRate; }
    [[nodiscard]] double getMobility() const { return mobility; }
    [[nodiscard]] const std::vector<double>& getResourceUsePerNiche() const { return resourceUsePerNiche; }
    [[nodiscard]] double getReproductivity() const { return reproductivity; }

    // Setters
    void setLocationId(int newLocationId) { locationId = newLocationId; }
    void setMutationRate(double newRate) { intrinsicMutationRate = newRate; }
    void setMobility(double newMobility) { mobility = newMobility; }
    void setResourceUsePerNiche(const std::vector<double>& resources) { resourceUsePerNiche = resources; }
    void setReproductivity(double newReproductionRate) { reproductivity = newReproductionRate; }

    // Utility methods
    void printDetails() const {
        std::cout << "UnitPopulation ID: " << unitPopulationId << ", Location ID: " << locationId
                  << ", Parent Unit ID: " << (parentUnitId ? std::to_string(*parentUnitId) : "None")
                  << ", Mutation Rate: " << intrinsicMutationRate << ", Mobility: " << mobility
                  << ", Reproductivity: " << reproductivity << std::endl;
    }
};


#endif //FUSION_UNIT_POPULATION_H
