#include "VehicleAssigner.h"
#include "HungarianSolver.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <unordered_map>

struct VehicleState {
    Vehicle vehicle;
    int used = 0;
    double totalDistance = 0.0;
    int currentNode = 0; // changes after every assigned delivery
};

static std::vector<VehicleState> makeVehicleStates(
    const std::vector<Vehicle>& vehicles,
    const std::vector<DeliveryTask>& tasks
) {
    std::unordered_map<std::string, int> warehouseNode;

    for (const DeliveryTask& task : tasks) {
        warehouseNode[task.warehouse_id] = task.warehouse_node;
    }

    std::vector<VehicleState> states;

    for (const Vehicle& vehicle : vehicles) {
        VehicleState state;
        state.vehicle = vehicle;

        if (warehouseNode.find(vehicle.warehouse_id) != warehouseNode.end()) {
            state.currentNode = warehouseNode[vehicle.warehouse_id];
        }

        states.push_back(state);
    }

    return states;
}

static RouteRecord makeUndelivered(const std::string& strategy, const DeliveryTask& task) {
    return {strategy, "NA", task.warehouse_id, task.stop_id, 1, 0.0, 0.0, 0.0, "UNDELIVERED"};
}

static RouteRecord assignToVehicle(
    const std::string& strategy,
    VehicleState& state,
    const DeliveryTask& task,
    double legDistance
) {
    state.used++;
    state.totalDistance += legDistance;
    state.currentNode = task.stop_node;

    double cost = legDistance * state.vehicle.cost_per_km;
    double utilization = (static_cast<double>(state.used) / state.vehicle.capacity) * 100.0;
    // Simple delay rule: if the vehicle crosses its daily distance limit.
    std::string status = state.totalDistance > state.vehicle.max_distance_per_day ? "DELAYED" : "DELIVERED";

    return {
        strategy,
        state.vehicle.vehicle_id,
        task.warehouse_id,
        task.stop_id,
        1,
        legDistance,
        cost,
        utilization,
        status
    };
}

static void updateFinalUtilization(std::vector<RouteRecord>& routes, const std::vector<VehicleState>& states) {
    std::unordered_map<std::string, double> finalUtilization;

    for (const VehicleState& state : states) {
        finalUtilization[state.vehicle.vehicle_id] =
            (static_cast<double>(state.used) / state.vehicle.capacity) * 100.0;
    }

    for (RouteRecord& route : routes) {
        if (finalUtilization.find(route.vehicle_id) != finalUtilization.end()) {
            route.utilization_percent = finalUtilization[route.vehicle_id];
        }
    }
}

static void addUnassignedPackages(
    AssignmentResult& result,
    const std::vector<DeliveryTask>& tasks,
    const std::vector<bool>& used
) {
    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        if (!used[i]) {
            result.routes.push_back(makeUndelivered(result.strategy, tasks[i]));
        }
    }
}

AssignmentResult VehicleAssigner::randomAssignment(
    const std::vector<DeliveryTask>& tasks,
    const std::vector<Vehicle>& vehicles,
    RoutePlanner& planner
) {
    AssignmentResult result{"Random"};

    std::vector<VehicleState> states = makeVehicleStates(vehicles, tasks);
    std::vector<int> taskOrder(tasks.size());
    std::vector<int> vehicleOrder(states.size());

    std::iota(taskOrder.begin(), taskOrder.end(), 0);
    std::iota(vehicleOrder.begin(), vehicleOrder.end(), 0);

    std::mt19937 rng(27);
    std::shuffle(taskOrder.begin(), taskOrder.end(), rng);
    std::shuffle(vehicleOrder.begin(), vehicleOrder.end(), rng);

    for (int taskIndex : taskOrder) {
        const DeliveryTask& task = tasks[taskIndex];
        bool assigned = false;

        for (int vehicleIndex : vehicleOrder) {
            VehicleState& state = states[vehicleIndex];

            if (!task.reachable ||
                state.vehicle.warehouse_id != task.warehouse_id ||
                state.used >= state.vehicle.capacity) {
                continue;
            }

            double legDistance = planner.distanceBetween(state.currentNode, task.stop_node);
            if (legDistance >= INF_DISTANCE / 2) {
                continue;
            }

            result.routes.push_back(assignToVehicle(result.strategy, state, task, legDistance));
            assigned = true;
            break;
        }

        if (!assigned) {
            result.routes.push_back(makeUndelivered(result.strategy, task));
        }
    }

    updateFinalUtilization(result.routes, states);
    return result;
}

AssignmentResult VehicleAssigner::nearestNeighbor(
    const std::vector<DeliveryTask>& tasks,
    const std::vector<Vehicle>& vehicles,
    RoutePlanner& planner
) {
    AssignmentResult result;
    result.strategy = "Nearest Neighbor";

    std::vector<VehicleState> states = makeVehicleStates(vehicles, tasks);
    std::vector<bool> usedTask(tasks.size(), false);

    for (VehicleState& state : states) {
        // Each vehicle keeps taking the closest package from its current location.
        while (state.used < state.vehicle.capacity) {
            int bestTask = -1;
            double bestDistance = INF_DISTANCE;

            for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
                if (usedTask[i] || !tasks[i].reachable ||
                    tasks[i].warehouse_id != state.vehicle.warehouse_id) {
                    continue;
                }

                double distance = planner.distanceBetween(state.currentNode, tasks[i].stop_node);
                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestTask = i;
                }
            }

            if (bestTask == -1) {
                break;
            }

            usedTask[bestTask] = true;
            result.routes.push_back(assignToVehicle(result.strategy, state, tasks[bestTask], bestDistance));
        }
    }

    addUnassignedPackages(result, tasks, usedTask);

    updateFinalUtilization(result.routes, states);
    return result;
}

AssignmentResult VehicleAssigner::optimizedGreedy(
    const std::vector<DeliveryTask>& tasks,
    const std::vector<Vehicle>& vehicles,
    RoutePlanner& planner
) {
    AssignmentResult result;
    result.strategy = "Optimized Greedy";

    std::vector<VehicleState> states = makeVehicleStates(vehicles, tasks);
    std::vector<bool> usedTask(tasks.size(), false);

    while (true) {
        int bestTask = -1;
        int bestVehicle = -1;
        double bestScore = INF_DISTANCE;
        double bestLegDistance = INF_DISTANCE;

        // Greedy choice: choose the best vehicle-package pair available right now.
        for (int i = 0; i < static_cast<int>(states.size()); i++) {
            VehicleState& state = states[i];

            if (state.used >= state.vehicle.capacity) {
                continue;
            }

            for (int j = 0; j < static_cast<int>(tasks.size()); j++) {
                const DeliveryTask& task = tasks[j];

                if (usedTask[j] || !task.reachable ||
                    task.warehouse_id != state.vehicle.warehouse_id) {
                    continue;
                }

                double legDistance = planner.distanceBetween(state.currentNode, task.stop_node);
                if (legDistance >= INF_DISTANCE / 2) {
                    continue;
                }

                double projectedDistance = state.totalDistance + legDistance;
                double delayPenalty = projectedDistance > state.vehicle.max_distance_per_day ? 5000.0 : 0.0;
                // Priority gives a small push, but distance and cost still matter more.
                double priorityBonus = task.priority * 2.0;
                double score = (legDistance * state.vehicle.cost_per_km) + delayPenalty - priorityBonus;

                if (score < bestScore) {
                    bestScore = score;
                    bestTask = j;
                    bestVehicle = i;
                    bestLegDistance = legDistance;
                }
            }
        }

        if (bestTask == -1 || bestVehicle == -1) {
            break;
        }

        usedTask[bestTask] = true;
        result.routes.push_back(assignToVehicle(result.strategy, states[bestVehicle], tasks[bestTask], bestLegDistance));
    }

    addUnassignedPackages(result, tasks, usedTask);

    updateFinalUtilization(result.routes, states);
    return result;
}
