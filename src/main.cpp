#include "CsvReader.h"
#include "Metrics.h"
#include "RoutePlanner.h"
#include "VehicleAssigner.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

template <typename Func>
AssignmentResult runWithTime(Func func, double& runtimeMs) {
    auto start = std::chrono::high_resolution_clock::now();
    AssignmentResult result = func();
    auto end = std::chrono::high_resolution_clock::now();

    runtimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    return result;
}

void waitBeforeExit() {
    std::cout << "\nPress Enter to close...";
    std::string temp;
    std::getline(std::cin, temp);
}

int main(int argc, char* argv[]) {
    // Helps when the exe is started from another folder by mistake.
    if (!std::filesystem::exists("data/roads.csv") && argc > 0) {
        std::filesystem::path exeFolder = std::filesystem::absolute(argv[0]).parent_path();

        if (std::filesystem::exists(exeFolder / "data" / "roads.csv")) {
            std::filesystem::current_path(exeFolder);
        }
    }

    Graph city;

    CsvReader::loadRoads("data/roads.csv", city);
    std::vector<Warehouse> warehouses = CsvReader::loadWarehouses("data/warehouses.csv");
    std::vector<Stop> stops = CsvReader::loadStops("data/stops.csv");
    std::vector<Vehicle> vehicles = CsvReader::loadVehicles("data/vehicles.csv");
    std::vector<Package> packages = CsvReader::loadPackages("data/packages.csv");

    if (warehouses.empty() || stops.empty() || vehicles.empty() || packages.empty()) {
        std::cout << "Some input data is missing. Please check the data folder.\n";
        waitBeforeExit();
        return 1;
    }

    std::cout << "Last-Mile Delivery Route Optimizer\n";
    std::cout << "----------------------------------\n";
    std::cout << "1. Run comparison with CSV input\n";
    std::cout << "2. Change input by adding one package, then run comparison\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "0") {
        std::cout << "Exiting program.\n";
        waitBeforeExit();
        return 0;
    }

    if (choice == "2") {
        std::string warehouseId, stopId, priorityText;

        std::cout << "\nEnter warehouse id (example W1 or W2): ";
        std::getline(std::cin, warehouseId);

        std::cout << "Enter stop id (example S1, S2, ...): ";
        std::getline(std::cin, stopId);

        std::cout << "Enter package priority (1 to 3): ";
        std::getline(std::cin, priorityText);

        bool warehouseFound = false;
        bool stopFound = false;

        for (const Warehouse& warehouse : warehouses) {
            if (warehouse.warehouse_id == warehouseId) {
                warehouseFound = true;
            }
        }

        for (const Stop& stop : stops) {
            if (stop.stop_id == stopId) {
                stopFound = true;
            }
        }

        int priority = 1;
        try {
            priority = std::stoi(priorityText);
        } catch (...) {
            priority = 1;
        }

        if (priority < 1 || priority > 3) {
            priority = 1;
        }

        if (warehouseFound && stopFound) {
            packages.push_back({"USER_PACKAGE", warehouseId, stopId, priority});
            std::cout << "Added USER_PACKAGE for this run only.\n";
        } else {
            std::cout << "Invalid warehouse or stop id. Running with CSV input only.\n";
        }
    }

    RoutePlanner planner(city);
    std::vector<DeliveryTask> tasks = planner.makeDeliveryTasks(packages, warehouses, stops);

    double randomMs = 0.0;
    double nearestMs = 0.0;
    double optimizedMs = 0.0;
    double hungarianMs = 0.0;

    AssignmentResult randomResult;
    AssignmentResult nearestResult;
    AssignmentResult optimizedResult;

    randomResult = runWithTime([&]() {
        return VehicleAssigner::randomAssignment(tasks, vehicles, planner);
    }, randomMs);

    nearestResult = runWithTime([&]() {
        return VehicleAssigner::nearestNeighbor(tasks, vehicles, planner);
    }, nearestMs);

    optimizedResult = runWithTime([&]() {
        return VehicleAssigner::optimizedGreedy(tasks, vehicles, planner);
    }, optimizedMs);

    AssignmentResult hungarianResult = runWithTime([&]() {
        return VehicleAssigner::hungarianAlgorithm(tasks, vehicles, planner);
    }, hungarianMs);

    std::vector<StrategyMetrics> summaries;
    summaries.push_back(Metrics::calculate(randomResult, vehicles, packages.size(), randomMs));
    summaries.push_back(Metrics::calculate(nearestResult, vehicles, packages.size(), nearestMs));
    summaries.push_back(Metrics::calculate(optimizedResult, vehicles, packages.size(), optimizedMs));
    summaries.push_back(Metrics::calculate(hungarianResult, vehicles, packages.size(), hungarianMs));

    std::cout << "\nShortest paths + vehicle assignment comparison\n";
    std::cout << "Hungarian Algorithm is highlighted as the lowest-cost assignment method.\n";
    std::cout << "Packages loaded: " << packages.size() << "\n";
    std::cout << "Vehicles loaded: " << vehicles.size() << "\n";

    Metrics::printComparison(summaries);

    std::filesystem::create_directories("outputs");

    Metrics::saveRouteSummary("outputs/route_summary.csv", {
        randomResult,
        nearestResult,
        optimizedResult,
        hungarianResult
    });

    Metrics::saveComparisonSummary("outputs/comparison_summary.txt", summaries);

    std::cout << "\nSaved route details to outputs/route_summary.csv\n";
    std::cout << "Saved comparison summary to outputs/comparison_summary.txt\n";

    waitBeforeExit();
    return 0;
}
