#ifndef METRICS_H
#define METRICS_H

#include "VehicleAssigner.h"

#include <string>
#include <vector>

struct StrategyMetrics {
    std::string strategy;
    double total_delivery_distance;
    double total_delivery_cost;
    double average_distance_per_package;
    int delivered_packages;
    int undelivered_packages;
    double average_vehicle_utilization;
    int delayed_deliveries;
    double runtime_ms;
};

class Metrics {
public:
    static StrategyMetrics calculate(
        const AssignmentResult& result,
        const std::vector<Vehicle>& vehicles,
        int totalPackages,
        double runtimeMs
    );

    static void printComparison(const std::vector<StrategyMetrics>& metrics);
    static void saveRouteSummary(const std::string& filename, const std::vector<AssignmentResult>& results);
    static void saveComparisonSummary(const std::string& filename, const std::vector<StrategyMetrics>& metrics);
};

#endif
