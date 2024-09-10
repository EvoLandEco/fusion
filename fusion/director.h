//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for director, which manipulates the whole system in a Monte-Carlo simulation manner
//

#ifndef FUSION_DIRECTOR_H
#define FUSION_DIRECTOR_H


#include <random>
#include <iostream>
#include "unit_population.h"
#include "isolation.h"
#include "event.h"
#include "system.h"
#include "observer.h"

class Director {
private:
    System system;                               // The system that holds isolations and populations
    double baseBirthRate;                        // Base birth rate (used for population events)
    double baseDeathRate;                        // Base death rate (used for population events)
    Observer observer;                           // Observer to record events
    std::vector<std::pair<int, PopulationEvent*>> populationEvents; // List of population events with corresponding rates
    std::vector<std::pair<int, IsolationEvent*>> isolationEvents;   // List of isolation events with corresponding rates
    std::random_device rd;                       // Random device for seeding random number generator
    std::mt19937 gen;                            // Random number generator

public:
    // Constructor to initialize the system and rates
    Director(int numIsolations, double birthRate, double deathRate)
            : system(numIsolations, birthRate, deathRate),
              baseBirthRate(birthRate), baseDeathRate(deathRate), gen(rd()) {}

    // Method to compute the rate for each event
    void computeEventRates() {
        // Clear previous events
        populationEvents.clear();
        isolationEvents.clear();

        // Loop over all isolations
        for (int isolationIndex = 0; isolationIndex < system.getNumberOfIsolations(); ++isolationIndex) {
            auto isolation = system.getIsolation(isolationIndex); // Use shared_ptr

            // For each population in the isolation, compute birth and death event rates
            const std::vector<UnitPopulation>& populations = isolation->getUnitPopulations();
            for (const UnitPopulation& population : populations) {
                // Birth event
                auto birthEvent = std::make_shared<PopulationBirthEvent>(&population, isolation, baseBirthRate);
                populationEvents.emplace_back(baseBirthRate, birthEvent.get());

                // Death event
                auto deathEvent = std::make_shared<PopulationDeathEvent>(&population, isolation);
                populationEvents.emplace_back(baseDeathRate, deathEvent.get());

                // Immigration events (considering barriers)
                for (int targetIsolationIndex = 0; targetIsolationIndex < system.getNumberOfIsolations(); ++targetIsolationIndex) {
                    if (isolationIndex != targetIsolationIndex) {
                        double barrier = system.getBarrierThreshold(isolationIndex, targetIsolationIndex);
                        if (barrier < 1.0) {
                            auto targetIsolation = system.getIsolation(targetIsolationIndex); // Use shared_ptr

                            auto immigrationEvent = std::make_shared<PopulationImmigrationEvent>(
                                    &population, isolation, targetIsolation, baseBirthRate
                            );
                            populationEvents.emplace_back(baseBirthRate * (1.0 - barrier), immigrationEvent.get());
                        }
                    }
                }
            }

            // Isolation resource change event (as an example)
            auto resourceChangeEvent = std::make_shared<ResourceAvailabilityChangeEvent>(
                    isolation, std::vector<double>{10.0, 10.0, 10.0} // Just an example
            );
            isolationEvents.emplace_back(0.1, resourceChangeEvent.get()); // Assuming a small probability for resource change

            // Barrier threshold change event
            auto barrierChangeEvent = std::make_shared<BarrierThresholdChangeEvent>(isolation, 0.5); // Example change
            isolationEvents.emplace_back(0.05, barrierChangeEvent.get()); // Assuming a smaller chance for barrier change
        }
    }


    // Method to sample the time for the next event and the event itself
    std::pair<double, Event*> sampleNextEvent() {
        // Compute total rate for all events
        double totalRate = 0.0;
        for (const auto& eventPair : populationEvents) {
            totalRate += eventPair.first;
        }
        for (const auto& eventPair : isolationEvents) {
            totalRate += eventPair.first;
        }

        // Sample the waiting time for the next event (exponentially distributed)
        std::exponential_distribution<> expDist(totalRate);
        double waitingTime = expDist(gen);

        // Sample which event happens (based on their relative rates)
        std::uniform_real_distribution<> uniformDist(0.0, totalRate);
        double threshold = uniformDist(gen);

        // Select event based on threshold
        double cumulativeRate = 0.0;
        for (const auto& eventPair : populationEvents) {
            cumulativeRate += eventPair.first;
            if (cumulativeRate >= threshold) {
                return {waitingTime, eventPair.second};
            }
        }
        for (const auto& eventPair : isolationEvents) {
            cumulativeRate += eventPair.first;
            if (cumulativeRate >= threshold) {
                return {waitingTime, eventPair.second};
            }
        }

        return {waitingTime, nullptr}; // Shouldn't reach here
    }

    // Method to run the simulation
    void runSimulation(double maxTime) {
        double currentTime = 0.0;

        // Main simulation loop
        while (currentTime < maxTime) {
            // Compute event rates for the current system state
            computeEventRates();

            // Sample the next event and the waiting time for it
            auto [waitingTime, nextEvent] = sampleNextEvent();

            // Update time
            currentTime += waitingTime;

            // Execute the next event and log it to the observer
            if (nextEvent) {
                if (dynamic_cast<PopulationBirthEvent*>(nextEvent)) {
                    auto* birthEvent = dynamic_cast<PopulationBirthEvent*>(nextEvent);
                    observer.logBirthEvent(currentTime, birthEvent->getParentId(), birthEvent->getChildId(), birthEvent->getLocationId());
                } else if (dynamic_cast<PopulationDeathEvent*>(nextEvent)) {
                    auto* deathEvent = dynamic_cast<PopulationDeathEvent*>(nextEvent);
                    observer.logDeathEvent(currentTime, deathEvent->getPopulationId());
                } else if (dynamic_cast<PopulationImmigrationEvent*>(nextEvent)) {
                    auto* immigrationEvent = dynamic_cast<PopulationImmigrationEvent*>(nextEvent);
                    observer.logImmigrationEvent(currentTime, immigrationEvent->getPopulationId(), immigrationEvent->getFromLocation(), immigrationEvent->getToLocation());
                } else if (dynamic_cast<PopulationMutationEvent*>(nextEvent)) {
                    auto* mutationEvent = dynamic_cast<PopulationMutationEvent*>(nextEvent);
                    observer.logMutationEvent(currentTime, mutationEvent->getPopulationId(), mutationEvent->getLocationId(),
                                              mutationEvent->getMutatedProperty(), mutationEvent->getOldValue(), mutationEvent->getNewValue());
                }

                nextEvent->execute(); // Execute event after logging
            }

            // Clean up memory (dynamic events)
            delete nextEvent;
        }

        // After simulation, print the history for review
        observer.printEventHistory();
    }
};


#endif //FUSION_DIRECTOR_H
