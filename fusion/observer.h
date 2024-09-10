//
// Created by Tianjian Qin on 9/8/2024.
// Class definition for observer, which observes literally and logs event history
//

#ifndef FUSION_OBSERVER_H
#define FUSION_OBSERVER_H


#include <string>
#include <vector>
#include <iostream>
#include <memory>

class Observer {
private:
    struct EventRecord {
        std::string eventType;          // Event type name (birth, death, immigration, mutation, etc.)
        double eventTime;               // Time at which the event occurred
        std::string eventDetails;       // Additional details about the event

        EventRecord(const std::string& type, double time, const std::string& details)
                : eventType(type), eventTime(time), eventDetails(details) {}
    };

    std::vector<EventRecord> eventHistory; // Vector to store all historical event records

public:
    // Log a birth event (with parent and child population details)
    void logBirthEvent(double time, int parentId, int childId, int locationId) {
        std::string details = "Parent ID: " + std::to_string(parentId) +
                              ", Child ID: " + std::to_string(childId) +
                              ", Location ID: " + std::to_string(locationId);
        eventHistory.emplace_back("Birth", time, details);
    }

    // Log a death event (with the population id that died)
    void logDeathEvent(double time, int populationId) {
        std::string details = "Population ID: " + std::to_string(populationId);
        eventHistory.emplace_back("Death", time, details);
    }

    // Log an immigration event (with population id, from location, and to location)
    void logImmigrationEvent(double time, int populationId, int fromLocationId, int toLocationId) {
        std::string details = "Population ID: " + std::to_string(populationId) +
                              ", From Location: " + std::to_string(fromLocationId) +
                              ", To Location: " + std::to_string(toLocationId);
        eventHistory.emplace_back("Immigration", time, details);
    }

    // Log a mutation event (with population id, location, mutated property and values)
    void logMutationEvent(double time, int populationId, int locationId,
                          const std::string& property, double oldValue, double newValue) {
        std::string details = "Population ID: " + std::to_string(populationId) +
                              ", Location ID: " + std::to_string(locationId) +
                              ", Mutated Property: " + property +
                              ", From Value: " + std::to_string(oldValue) +
                              ", To Value: " + std::to_string(newValue);
        eventHistory.emplace_back("Mutation", time, details);
    }

    // Get the entire event history for further processing
    const std::vector<EventRecord>& getEventHistory() const {
        return eventHistory;
    }

    // Print all logged events (can be replaced by more advanced data handling or exporting)
    void printEventHistory() const {
        std::cout << "Event History:\n";
        for (const auto& record : eventHistory) {
            std::cout << "Time: " << record.eventTime << " | Type: " << record.eventType
                      << " | Details: " << record.eventDetails << "\n";
        }
    }

    // Clear event history (optional, for resetting the observer)
    void clearHistory() {
        eventHistory.clear();
    }
};


#endif //FUSION_OBSERVER_H
