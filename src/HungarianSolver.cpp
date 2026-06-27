#include "HungarianSolver.h"
#include "VehicleAssigner.h"

#include <algorithm>
#include <unordered_map>

struct HungarianVehicleState {
    Vehicle vehicle;
    int used = 0;
    double totalDistance = 0.0;
    int currentNode = 0;
};

static RouteRecord hungarianUndelivered(const DeliveryTask& task) {
    return {"Hungarian Algorithm", "NA", task.warehouse_id, task.stop_id, 1, 0.0, 0.0, 0.0, "UNDELIVERED"};
}

static RouteRecord hungarianDeliver(
    HungarianVehicleState& state,
    const DeliveryTask& task,
    double distance
) {
    state.used++;
    state.totalDistance += distance;
    state.currentNode = task.stop_node;

    double cost = distance * state.vehicle.cost_per_km;
    double usage = (static_cast<double>(state.used) / state.vehicle.capacity) * 100.0;
    std::string status = state.totalDistance > state.vehicle.max_distance_per_day ? "DELAYED" : "DELIVERED";

    return {"Hungarian Algorithm", state.vehicle.vehicle_id, task.warehouse_id, task.stop_id, 1, distance, cost, usage, status};
}

std::vector<int> solveHungarian(const std::vector<std::vector<double>>& cost, double bigCost) {
    int rows = static_cast<int>(cost.size()) - 1;
    int cols = static_cast<int>(cost[0].size()) - 1;

    std::vector<double> u(rows + 1), v(cols + 1);
    std::vector<int> p(cols + 1), way(cols + 1);

    for (int i = 1; i <= rows; i++) {
        p[0] = i;
        int j0 = 0;
        std::vector<double> minv(cols + 1, bigCost);
        std::vector<bool> used(cols + 1, false);

        do {
            used[j0] = true;
            int i0 = p[j0];
            double delta = bigCost;
            int j1 = 0;

            for (int j = 1; j <= cols; j++) {
                if (used[j]) {
                    continue;
                }

                double current = cost[i0][j] - u[i0] - v[j];

                if (current < minv[j]) {
                    minv[j] = current;
                    way[j] = j0;
                }

                if (minv[j] < delta) {
                    delta = minv[j];
                    j1 = j;
                }
            }

            for (int j = 0; j <= cols; j++) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }

            j0 = j1;
        } while (p[j0] != 0);

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }

    std::vector<int> assignedColForRow(rows + 1, 0);

    for (int j = 1; j <= cols; j++) {
        if (p[j] != 0) {
            assignedColForRow[p[j]] = j;
        }
    }

    return assignedColForRow;
}

AssignmentResult VehicleAssigner::hungarianAlgorithm(
    const std::vector<DeliveryTask>& tasks,
    const std::vector<Vehicle>& vehicles,
    RoutePlanner& planner
) {
    AssignmentResult result{"Hungarian Algorithm"};
    std::unordered_map<std::string, int> warehouseNode;
    std::vector<HungarianVehicleState> states;
    std::vector<int> taskIds, slotVehicle;
    const double BIG = 1e12;

    for (const DeliveryTask& task : tasks) {
        warehouseNode[task.warehouse_id] = task.warehouse_node;
    }

    for (const Vehicle& vehicle : vehicles) {
        states.push_back({vehicle, 0, 0.0, warehouseNode[vehicle.warehouse_id]});
    }

    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        if (tasks[i].reachable) {
            taskIds.push_back(i);
        }
    }

    for (int i = 0; i < static_cast<int>(states.size()); i++) {
        for (int c = 0; c < states[i].vehicle.capacity; c++) {
            slotVehicle.push_back(i);
        }
    }

    int rows = static_cast<int>(taskIds.size());
    int cols = std::max(rows, static_cast<int>(slotVehicle.size()));
    std::vector<std::vector<double>> cost(rows + 1, std::vector<double>(cols + 1, BIG));

    for (int i = 1; i <= rows; i++) {
        const DeliveryTask& task = tasks[taskIds[i - 1]];

        for (int j = 1; j <= static_cast<int>(slotVehicle.size()); j++) {
            HungarianVehicleState& state = states[slotVehicle[j - 1]];

            if (state.vehicle.warehouse_id == task.warehouse_id) {
                double distance = planner.distanceBetween(task.warehouse_node, task.stop_node);
                cost[i][j] = (distance * state.vehicle.cost_per_km) - (task.priority * 2.0);
            }
        }
    }

    std::vector<int> assignedSlot = solveHungarian(cost, BIG);
    std::vector<bool> delivered(tasks.size(), false);
    std::vector<std::vector<int>> vehicleTasks(states.size());

    for (int i = 1; i <= rows; i++) {
        int slot = assignedSlot[i];

        if (slot > 0 && slot <= static_cast<int>(slotVehicle.size()) && cost[i][slot] < BIG / 2) {
            vehicleTasks[slotVehicle[slot - 1]].push_back(taskIds[i - 1]);
        }
    }

    for (int i = 0; i < static_cast<int>(states.size()); i++) {
        while (!vehicleTasks[i].empty()) {
            int bestPos = 0;
            double bestDistance = INF_DISTANCE;

            for (int j = 0; j < static_cast<int>(vehicleTasks[i].size()); j++) {
                int taskIndex = vehicleTasks[i][j];
                double distance = planner.distanceBetween(states[i].currentNode, tasks[taskIndex].stop_node);

                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestPos = j;
                }
            }

            int taskIndex = vehicleTasks[i][bestPos];
            delivered[taskIndex] = true;
            result.routes.push_back(hungarianDeliver(states[i], tasks[taskIndex], bestDistance));
            vehicleTasks[i].erase(vehicleTasks[i].begin() + bestPos);
        }
    }

    for (int i = 0; i < static_cast<int>(tasks.size()); i++) {
        if (!delivered[i]) {
            result.routes.push_back(hungarianUndelivered(tasks[i]));
        }
    }

    return result;
}
