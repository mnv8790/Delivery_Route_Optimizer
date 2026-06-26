#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include "CsvReader.h"
#include "Graph.h"

#include <string>
#include <unordered_map>
#include <vector>

const double INF_DISTANCE = 1e15;

struct DeliveryTask {
    std::string package_id;
    std::string warehouse_id;
    std::string stop_id;
    int priority;
    int warehouse_node;
    int stop_node;
    double distance_from_warehouse;
    bool reachable;
};

class RoutePlanner {
private:
    const Graph& graph;
    std::unordered_map<int, std::unordered_map<int, double>> shortestCache;

public:
    RoutePlanner(const Graph& graph);

    double distanceBetween(int fromNode, int toNode);
    std::vector<DeliveryTask> makeDeliveryTasks(
        const std::vector<Package>& packages,
        const std::vector<Warehouse>& warehouses,
        const std::vector<Stop>& stops
    );
};

#endif
