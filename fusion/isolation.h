//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for isolation, an abstract island-like entity to accommodate species/populations isolated from other
//

#ifndef FUSION_ISOLATION_H
#define FUSION_ISOLATION_H


#include <utility>
#include <vector>
#include <iostream>
#include "unit_population.h"
#include "niche.h"

class Isolation {
private:
    NicheSpaces nicheSpaces;                             // List of available and occupied spaces for all niche dimensions
    std::vector<UnitPopulation> unitPopulations{};       // List of populations on this island

public:
    // Constructor
    explicit Isolation(NicheSpaces niches)
            : nicheSpaces(std::move(niches)) {}

    // Destructor
    ~Isolation() = default;

    // Add a new UnitPopulation to the island
    void addUnitPopulation(const UnitPopulation& population) {
        unitPopulations.push_back(population);
    }

    // Get the list of UnitPopulations
    [[nodiscard]] const std::vector<UnitPopulation>& getUnitPopulations() const {
        return unitPopulations;
    }

    // Get the niche spaces
    [[nodiscard]] const NicheSpaces& getNicheSpaces() const {
        return nicheSpaces;
    }

    // Modify the entire niche spaces
    void setNicheSpaces(const NicheSpaces& niches) {
        nicheSpaces = niches;
    }

    // Utility method to print island details
    void printDetails() const {
        std::cout << "Niche Space Details:\n";
        nicheSpaces.printDetails();

        std::cout << "Unit Populations on this Island:\n";
        for (const auto& population : unitPopulations) {
            population.printDetails();
        }
    }
};


#endif //FUSION_ISOLATION_H
