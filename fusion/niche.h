//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for niche spaces, an abstract resource capacity space
//

#ifndef FUSION_NICHE_H
#define FUSION_NICHE_H

#include <iostream>
#include <vector>

// Class representing a single niche dimension
struct NicheDimension {
    double occupiedSpace;
    double availableSpace;

    NicheDimension(double occupied, double available)
            : occupiedSpace(occupied), availableSpace(available) {}
};

// Class representing multiple niche dimensions
class NicheSpaces {
private:
    std::vector<NicheDimension> dimensions;

public:
    // Constructor
    NicheSpaces(const std::vector<std::pair<double, double>>& niches) {
        for (const auto& niche : niches) {
            dimensions.emplace_back(niche.first, niche.second);
        }
    }

    // Get the number of niche dimensions
    [[nodiscard]] size_t getNumberOfDimensions() const {
        return dimensions.size();
    }

    // Get occupied space for a specific dimension
    [[nodiscard]] double getOccupiedSpace(size_t index) const {
        if (index < dimensions.size()) {
            return dimensions[index].occupiedSpace;
        }
        throw std::out_of_range("Index out of bounds");
    }

    // Get available space for a specific dimension
    [[nodiscard]] double getAvailableSpace(size_t index) const {
        if (index < dimensions.size()) {
            return dimensions[index].availableSpace;
        }
        throw std::out_of_range("Index out of bounds");
    }

    // Set occupied space for a specific dimension
    void setOccupiedSpace(size_t index, double occupied) {
        if (index < dimensions.size()) {
            dimensions[index].occupiedSpace = occupied;
        } else {
            throw std::out_of_range("Index out of bounds");
        }
    }

    // Set available space for a specific dimension
    void setAvailableSpace(size_t index, double available) {
        if (index < dimensions.size()) {
            dimensions[index].availableSpace = available;
        } else {
            throw std::out_of_range("Index out of bounds");
        }
    }

    // Print details of the niche dimensions
    void printDetails() const {
        std::cout << "Niche Dimensions:\n";
        for (size_t i = 0; i < dimensions.size(); ++i) {
            std::cout << "Dimension " << i << ": Occupied Space = " << dimensions[i].occupiedSpace
                      << ", Available Space = " << dimensions[i].availableSpace << std::endl;
        }
    }
};


#endif //FUSION_NICHE_H
