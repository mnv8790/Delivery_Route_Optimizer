#include "ConsoleHelper.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

void setProjectFolder(int argc, char* argv[]) {
    if (!std::filesystem::exists("data/roads.csv") && argc > 0) {
        std::filesystem::path exeFolder = std::filesystem::absolute(argv[0]).parent_path();

        if (std::filesystem::exists(exeFolder / "data" / "roads.csv")) {
            std::filesystem::current_path(exeFolder);
        }
    }
}

void waitBeforeExit() {
    std::cout << "\nPress Enter to close...";
    std::string temp;
    std::getline(std::cin, temp);
}

void printMenu() {
    std::cout << "Last-Mile Delivery Route Optimizer\n";
    std::cout << "----------------------------------\n";
    std::cout << "1. Run comparison with CSV input\n";
    std::cout << "2. Change input by adding one package, then run comparison\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

void addPackageFromUser(
    std::vector<Package>& packages,
    const std::vector<Warehouse>& warehouses,
    const std::vector<Stop>& stops
) {
    std::string warehouseId, stopId, priorityText;

    std::cout << "\nEnter warehouse id (example W1 or W2): ";
    std::getline(std::cin, warehouseId);

    std::cout << "Enter stop id (example S1, S2, ...): ";
    std::getline(std::cin, stopId);

    std::cout << "Enter package priority (1 to 3): ";
    std::getline(std::cin, priorityText);

    bool warehouseFound = std::any_of(warehouses.begin(), warehouses.end(), [&](const Warehouse& warehouse) {
        return warehouse.warehouse_id == warehouseId;
    });

    bool stopFound = std::any_of(stops.begin(), stops.end(), [&](const Stop& stop) {
        return stop.stop_id == stopId;
    });

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
