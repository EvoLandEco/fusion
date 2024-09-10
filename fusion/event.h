//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for possible events affecting either population units or isolations
//

#ifndef FUSION_EVENT_H
#define FUSION_EVENT_H


#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "unit_population.h"
#include "isolation.h"

// Event Base Class
class Event {
public:
    virtual ~Event() = default;
    virtual void execute() = 0;
};

// Population Event Base Class
class PopulationEvent : public Event {
protected:
    std::shared_ptr<UnitPopulation> population;   // Target population for the event
    std::shared_ptr<Isolation> isolation;         // The isolation (island) on which the event takes place
    double mutationRate;                          // Independent mutation rate

public:
    PopulationEvent(std::shared_ptr<UnitPopulation> pop, std::shared_ptr<Isolation> iso, double rate)
            : population(pop), isolation(iso), mutationRate(rate) {}

    virtual void execute() override = 0;
};

// Population Birth Event
class PopulationBirthEvent : public PopulationEvent {
public:
    PopulationBirthEvent(std::shared_ptr<UnitPopulation> parent, std::shared_ptr<Isolation> iso, double rate)
            : PopulationEvent(parent, iso, rate) {}

    void execute() override {
        // Spawn a child population
        auto child = std::make_shared<UnitPopulation>(population->getId() + 1, population->getLocationId(),
                                                      population->getId(), population->getMutationRate(),
                                                      population->getMobility(), population->getResourceUsePerNiche(),
                                                      population->getReproductivity());

        // Perform mutation with a certain probability
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        if (dis(gen) < mutationRate) {
            auto mutation = std::make_shared<PopulationMutationEvent>(child, mutationRate);
            mutation->execute();
        }

        // Add the child to the island
        isolation->addUnitPopulation(child);
        std::cout << "Population birth event executed.\n";
    }
};

// Population Death Event
class PopulationDeathEvent : public PopulationEvent {
public:
    PopulationDeathEvent(std::shared_ptr<UnitPopulation> pop, std::shared_ptr<Isolation> iso)
            : PopulationEvent(pop, iso, 0.0) {}

    void execute() override {
        // Remove the population from the island
        isolation->removeUnitPopulation(population->getId());
        std::cout << "Population death event executed.\n";
    }
};

// Population Immigration Event
class PopulationImmigrationEvent : public PopulationEvent {
private:
    std::shared_ptr<Isolation> targetIsolation;  // Target isolation (different island)

public:
    PopulationImmigrationEvent(std::shared_ptr<UnitPopulation> parent, std::shared_ptr<Isolation> srcIso,
                               std::shared_ptr<Isolation> tgtIso, double rate)
            : PopulationEvent(parent, srcIso, rate), targetIsolation(tgtIso) {}

    void execute() override {
        // Spawn a child population on the target island
        auto child = std::make_shared<UnitPopulation>(population->getId() + 1, targetIsolation->getId(),
                                                      population->getId(), population->getMutationRate(),
                                                      population->getMobility(), population->getResourceUsePerNiche(),
                                                      population->getReproductivity());

        // Perform mutation with a certain probability
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        if (dis(gen) < mutationRate) {
            auto mutation = std::make_shared<PopulationMutationEvent>(child, mutationRate);
            mutation->execute();
        }

        // Add the child to the target island
        targetIsolation->addUnitPopulation(child);
        std::cout << "Population immigration event executed.\n";
    }
};

// Population Mutation Event
class PopulationMutationEvent : public PopulationEvent {
public:
    PopulationMutationEvent(std::shared_ptr<UnitPopulation> pop, double rate)
            : PopulationEvent(pop, nullptr, rate) {}

    void execute() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> mutationType(0, 3);

        int mutation = mutationType(gen);
        switch (mutation) {
            case 0:
                population->setMobility(population->getMobility() + 0.1);  // Mobility mutation
                break;
            case 1:
                population->setResourceUsePerNiche({1.1});                 // Resource use mutation
                break;
            case 2:
                population->setReproductivity(population->getReproductivity() + 0.1); // Reproductivity mutation
                break;
            case 3:
                population->setMutationRate(population->getMutationRate() + 0.01); // Intrinsic mutation rate mutation
                break;
        }
        std::cout << "Population mutation event executed.\n";
    }
};

// Isolation Event Base Class
class IsolationEvent : public Event {
protected:
    std::shared_ptr<Isolation> isolation; // Target isolation for the event

public:
    IsolationEvent(std::shared_ptr<Isolation> iso) : isolation(iso) {}

    virtual void execute() override = 0;
};

// Resource Availability Change Event
class ResourceAvailabilityChangeEvent : public IsolationEvent {
private:
    std::vector<double> newResourceAvailability;

public:
    ResourceAvailabilityChangeEvent(std::shared_ptr<Isolation> iso, const std::vector<double>& resources)
            : IsolationEvent(iso), newResourceAvailability(resources) {}

    void execute() override {
        isolation->setResourceAvailability(newResourceAvailability);
        std::cout << "Resource availability change event executed.\n";
    }
};

// Barrier Threshold Change Event
class BarrierThresholdChangeEvent : public IsolationEvent {
private:
    double newThreshold;

public:
    BarrierThresholdChangeEvent(std::shared_ptr<Isolation> iso, double threshold)
            : IsolationEvent(iso), newThreshold(threshold) {}

    void execute() override {
        isolation->setBarrierThreshold(newThreshold);
        std::cout << "Barrier threshold change event executed.\n";
    }
};


#endif //FUSION_EVENT_H
