#include "RoutePlanner.h"

#include <iostream>

RoutePlanner::RoutePlanner(const Graph& graph) : graph(graph) {
}

double RoutePlanner::distanceBetween(int fromNode, int toNode) {
    // Cache shortest paths so we do not run Dijkstra again for the same start node.
    if (shortestCache.find(fromNode) == shortestCache.end()) {
        shortestCache[fromNode] = graph.shortestPaths(fromNode);
    }

    auto it = shortestCache[fromNode].find(toNode);
    if (it == shortestCache[fromNode].end()) {
        return INF_DISTANCE;
    }

    return it->second;
}

std::vector<DeliveryTask> RoutePlanner::makeDeliveryTasks(
    const std::vector<Package>& packages,
    const std::vector<Warehouse>& warehouses,
    const std::vector<Stop>& stops
) {
    std::unordered_map<std::string, int> warehouseNode;
    std::unordered_map<std::string, int> stopNode;

    for (const Warehouse& warehouse : warehouses) {
        warehouseNode[warehouse.warehouse_id] = warehouse.node_id;
    }

    for (const Stop& stop : stops) {
        stopNode[stop.stop_id] = stop.node_id;
    }

    std::vector<DeliveryTask> tasks;

    for (const Package& package : packages) {
        // Convert each package row into a route task with actual graph node numbers.
        if (warehouseNode.find(package.warehouse_id) == warehouseNode.end() ||
            stopNode.find(package.stop_id) == stopNode.end()) {
            std::cout << "Skipping package with missing warehouse/stop: "
                      << package.package_id << "\n";
            continue;
        }

        int startNode = warehouseNode[package.warehouse_id];
        int endNode = stopNode[package.stop_id];
        double distance = distanceBetween(startNode, endNode);
        bool reachable = distance < INF_DISTANCE / 2;

        tasks.push_back({
            package.package_id,
            package.warehouse_id,
            package.stop_id,
            package.priority,
            startNode,
            endNode,
            distance,
            reachable
        });
    }

    return tasks;
}
