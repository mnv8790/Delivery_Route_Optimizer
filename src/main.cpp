#include "ConsoleHelper.h"
#include "CsvReader.h"
#include "Metrics.h"
#include "RoutePlanner.h"
#include "VehicleAssigner.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    setProjectFolder(argc, argv);

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

    printMenu();

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "0") {
        std::cout << "Exiting program.\n";
        waitBeforeExit();
        return 0;
    }

    if (choice == "2") {
        addPackageFromUser(packages, warehouses, stops);
    }

    RoutePlanner planner(city);
    std::vector<DeliveryTask> tasks = planner.makeDeliveryTasks(packages, warehouses, stops);

    double randomMs = 0.0;
    double nearestMs = 0.0;
    double optimizedMs = 0.0;
    double hungarianMs = 0.0;

    auto run = [&](auto function, double& runtimeMs) {
        auto start = std::chrono::high_resolution_clock::now();
        AssignmentResult result = function(tasks, vehicles, planner);
        auto end = std::chrono::high_resolution_clock::now();
        runtimeMs = std::chrono::duration<double, std::milli>(end - start).count();
        return result;
    };

    AssignmentResult randomResult = run(VehicleAssigner::randomAssignment, randomMs);
    AssignmentResult nearestResult = run(VehicleAssigner::nearestNeighbor, nearestMs);
    AssignmentResult optimizedResult = run(VehicleAssigner::optimizedGreedy, optimizedMs);
    AssignmentResult hungarianResult = run(VehicleAssigner::hungarianAlgorithm, hungarianMs);

    std::vector<StrategyMetrics> summaries = {
        Metrics::calculate(randomResult, vehicles, packages.size(), randomMs),
        Metrics::calculate(nearestResult, vehicles, packages.size(), nearestMs),
        Metrics::calculate(optimizedResult, vehicles, packages.size(), optimizedMs),
        Metrics::calculate(hungarianResult, vehicles, packages.size(), hungarianMs)
    };

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
