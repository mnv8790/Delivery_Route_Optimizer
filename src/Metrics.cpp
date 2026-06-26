#include "Metrics.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

StrategyMetrics Metrics::calculate(
    const AssignmentResult& result,
    const std::vector<Vehicle>& vehicles,
    int totalPackages,
    double runtimeMs
) {
    double totalDistance = 0.0;
    double totalCost = 0.0;
    int delivered = 0;
    int delayed = 0;
    std::unordered_map<std::string, int> vehiclePackages;

    for (const RouteRecord& route : result.routes) {
        if (route.status == "UNDELIVERED") {
            continue;
        }

        // Only delivered or delayed packages are counted in distance and cost.
        delivered += route.package_count;
        totalDistance += route.distance_km;
        totalCost += route.cost;

        if (route.status == "DELAYED") {
            delayed += route.package_count;
        }

        vehiclePackages[route.vehicle_id] += route.package_count;
    }

    double utilizationSum = 0.0;

    for (const Vehicle& vehicle : vehicles) {
        double used = vehiclePackages[vehicle.vehicle_id];
        // Average utilization is calculated across all vehicles, even unused ones.
        utilizationSum += (used / vehicle.capacity) * 100.0;
    }

    StrategyMetrics m;
    m.strategy = result.strategy;
    m.total_delivery_distance = totalDistance;
    m.total_delivery_cost = totalCost;
    m.average_distance_per_package = delivered == 0 ? 0.0 : totalDistance / delivered;
    m.delivered_packages = delivered;
    m.undelivered_packages = totalPackages - delivered;
    m.average_vehicle_utilization = vehicles.empty() ? 0.0 : utilizationSum / vehicles.size();
    m.delayed_deliveries = delayed;
    m.runtime_ms = runtimeMs;

    return m;
}

void Metrics::printComparison(const std::vector<StrategyMetrics>& metrics) {
    if (metrics.empty()) {
        return;
    }

    std::cout << "\nComparison Summary\n";
    std::cout << "Strategy             Distance   Cost(Rs)   Avg/Package   Delivered   Undelivered   Util(%)   Delayed   Runtime(ms)\n";
    std::cout << "-----------------------------------------------------------------------------------------------------------------\n";

    for (const StrategyMetrics& m : metrics) {
        std::cout << std::left << std::setw(19) << m.strategy
                  << std::right << std::setw(9) << std::fixed << std::setprecision(1) << m.total_delivery_distance
                  << std::setw(11) << std::fixed << std::setprecision(0) << m.total_delivery_cost
                  << std::setw(14) << std::fixed << std::setprecision(2) << m.average_distance_per_package
                  << std::setw(12) << m.delivered_packages
                  << std::setw(14) << m.undelivered_packages
                  << std::setw(10) << std::fixed << std::setprecision(1) << m.average_vehicle_utilization
                  << std::setw(10) << m.delayed_deliveries
                  << std::setw(13) << std::fixed << std::setprecision(3) << m.runtime_ms << "\n";
    }

    const StrategyMetrics* random = nullptr;
    const StrategyMetrics* hungarian = nullptr;

    for (const StrategyMetrics& m : metrics) {
        if (m.strategy == "Random") {
            random = &m;
        }
        if (m.strategy == "Hungarian Algorithm") {
            hungarian = &m;
        }
    }

    if (random == nullptr || hungarian == nullptr) {
        std::cout << "\nRun full strategy comparison to see improvement over Random.\n";
        return;
    }

    double distanceImprovement = 0.0;
    double costImprovement = 0.0;
    double delayImprovement = 0.0;

    if (random->total_delivery_distance > 0) {
        distanceImprovement =
            ((random->total_delivery_distance - hungarian->total_delivery_distance) / random->total_delivery_distance) * 100.0;
    }

    if (random->total_delivery_cost > 0) {
        costImprovement =
            ((random->total_delivery_cost - hungarian->total_delivery_cost) / random->total_delivery_cost) * 100.0;
    }

    if (random->delayed_deliveries > 0) {
        delayImprovement =
            (static_cast<double>(random->delayed_deliveries - hungarian->delayed_deliveries) / random->delayed_deliveries) * 100.0;
    }

    std::cout << "\nImprovement over Random using Hungarian Algorithm\n";
    std::cout << "Distance reduced by: " << std::fixed << std::setprecision(1) << distanceImprovement << "%\n";
    std::cout << "Cost reduced by: " << std::fixed << std::setprecision(1) << costImprovement << "%\n";
    std::cout << "Delays reduced by: " << std::fixed << std::setprecision(1) << delayImprovement << "%\n";
}

void Metrics::saveRouteSummary(const std::string& filename, const std::vector<AssignmentResult>& results) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "Could not write " << filename << "\n";
        return;
    }

    file << "strategy,vehicle_id,warehouse_id,stop_id,package_count,distance_km,cost,utilization_percent,status\n";

    for (const AssignmentResult& result : results) {
        for (const RouteRecord& route : result.routes) {
            file << route.strategy << ","
                 << route.vehicle_id << ","
                 << route.warehouse_id << ","
                 << route.stop_id << ","
                 << route.package_count << ","
                 << std::fixed << std::setprecision(2) << route.distance_km << ","
                 << std::fixed << std::setprecision(2) << route.cost << ","
                 << std::fixed << std::setprecision(1) << route.utilization_percent << ","
                 << route.status << "\n";
        }
    }
}

void Metrics::saveComparisonSummary(const std::string& filename, const std::vector<StrategyMetrics>& metrics) {
    std::ofstream file(filename);

    if (!file.is_open() || metrics.empty()) {
        std::cout << "Could not write " << filename << "\n";
        return;
    }

    file << "Last-Mile Delivery Route Optimizer Summary\n\n";

    for (const StrategyMetrics& m : metrics) {
        file << m.strategy << " Assignment:\n";
        file << "- Total delivery distance: " << std::fixed << std::setprecision(1)
             << m.total_delivery_distance << " km\n";
        file << "- Total delivery cost: Rs. " << std::fixed << std::setprecision(0)
             << m.total_delivery_cost << "\n";
        file << "- Average distance per package: " << std::fixed << std::setprecision(2)
             << m.average_distance_per_package << " km\n";
        file << "- Packages delivered: " << m.delivered_packages << " / "
             << m.delivered_packages + m.undelivered_packages << "\n";
        file << "- Undelivered packages: " << m.undelivered_packages << "\n";
        file << "- Average vehicle utilization: " << std::fixed << std::setprecision(1)
             << m.average_vehicle_utilization << "%\n";
        file << "- Delayed deliveries: " << m.delayed_deliveries << "\n";
        file << "- Runtime: " << std::fixed << std::setprecision(3) << m.runtime_ms << " ms\n\n";
    }

    const StrategyMetrics* random = nullptr;
    const StrategyMetrics* hungarian = nullptr;

    for (const StrategyMetrics& m : metrics) {
        if (m.strategy == "Random") {
            random = &m;
        }
        if (m.strategy == "Hungarian Algorithm") {
            hungarian = &m;
        }
    }

    if (random == nullptr || hungarian == nullptr) {
        file << "Run full strategy comparison to see improvement percentages over Random.\n";
        return;
    }

    double distanceImprovement =
        random->total_delivery_distance == 0.0 ? 0.0 :
        ((random->total_delivery_distance - hungarian->total_delivery_distance) / random->total_delivery_distance) * 100.0;

    double costImprovement =
        random->total_delivery_cost == 0.0 ? 0.0 :
        ((random->total_delivery_cost - hungarian->total_delivery_cost) / random->total_delivery_cost) * 100.0;

    double delayImprovement =
        random->delayed_deliveries == 0 ? 0.0 :
        (static_cast<double>(random->delayed_deliveries - hungarian->delayed_deliveries) / random->delayed_deliveries) * 100.0;

    file << "Improvement over Random:\n";
    file << "- Distance reduced by: " << std::fixed << std::setprecision(1) << distanceImprovement << "%\n";
    file << "- Cost reduced by: " << std::fixed << std::setprecision(1) << costImprovement << "%\n";
    file << "- Delays reduced by: " << std::fixed << std::setprecision(1) << delayImprovement << "%\n\n";

    file << "Hungarian Algorithm performed well because it treats vehicle capacity slots "
         << "as assignment choices and minimizes the overall delivery cost table.\n";
}
