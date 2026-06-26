#ifndef VEHICLE_ASSIGNER_H
#define VEHICLE_ASSIGNER_H

#include "RoutePlanner.h"

#include <string>
#include <vector>

struct RouteRecord {
    std::string strategy;
    std::string vehicle_id;
    std::string warehouse_id;
    std::string stop_id;
    int package_count;
    double distance_km;
    double cost;
    double utilization_percent;
    std::string status;
};

struct AssignmentResult {
    std::string strategy;
    std::vector<RouteRecord> routes;
};

class VehicleAssigner {
public:
    static AssignmentResult randomAssignment(
        const std::vector<DeliveryTask>& tasks,
        const std::vector<Vehicle>& vehicles,
        RoutePlanner& planner
    );

    static AssignmentResult nearestNeighbor(
        const std::vector<DeliveryTask>& tasks,
        const std::vector<Vehicle>& vehicles,
        RoutePlanner& planner
    );

    static AssignmentResult optimizedGreedy(
        const std::vector<DeliveryTask>& tasks,
        const std::vector<Vehicle>& vehicles,
        RoutePlanner& planner
    );

    static AssignmentResult hungarianAlgorithm(
        const std::vector<DeliveryTask>& tasks,
        const std::vector<Vehicle>& vehicles,
        RoutePlanner& planner
    );
};

#endif
