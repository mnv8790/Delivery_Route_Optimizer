#include "CsvReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

void CsvReader::loadRoads(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "\n";
        return;
    }

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string fromText, toText, distanceText;

        getline(ss, fromText, ',');
        getline(ss, toText, ',');
        getline(ss, distanceText, ',');

        if (fromText.empty() || toText.empty() || distanceText.empty()) {
            continue;
        }

        graph.addEdge(std::stoi(fromText), std::stoi(toText), std::stod(distanceText));
    }
}

std::vector<Warehouse> CsvReader::loadWarehouses(const std::string& filename) {
    std::vector<Warehouse> warehouses;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "\n";
        return warehouses;
    }

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, nodeText;

        getline(ss, id, ',');
        getline(ss, nodeText, ',');

        if (!id.empty() && !nodeText.empty()) {
            warehouses.push_back({id, std::stoi(nodeText)});
        }
    }

    return warehouses;
}

std::vector<Stop> CsvReader::loadStops(const std::string& filename) {
    std::vector<Stop> stops;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "\n";
        return stops;
    }

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, nodeText;

        getline(ss, id, ',');
        getline(ss, nodeText, ',');

        if (!id.empty() && !nodeText.empty()) {
            stops.push_back({id, std::stoi(nodeText)});
        }
    }

    return stops;
}

std::vector<Vehicle> CsvReader::loadVehicles(const std::string& filename) {
    std::vector<Vehicle> vehicles;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "\n";
        return vehicles;
    }

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, warehouseId, capacityText, costText, maxDistanceText;

        getline(ss, id, ',');
        getline(ss, warehouseId, ',');
        getline(ss, capacityText, ',');
        getline(ss, costText, ',');
        getline(ss, maxDistanceText, ',');

        if (!id.empty() && !warehouseId.empty()) {
            vehicles.push_back({
                id,
                warehouseId,
                std::stoi(capacityText),
                std::stod(costText),
                std::stod(maxDistanceText)
            });
        }
    }

    return vehicles;
}

std::vector<Package> CsvReader::loadPackages(const std::string& filename) {
    std::vector<Package> packages;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Could not open " << filename << "\n";
        return packages;
    }

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, warehouseId, stopId, priorityText;

        getline(ss, id, ',');
        getline(ss, warehouseId, ',');
        getline(ss, stopId, ',');
        getline(ss, priorityText, ',');

        if (!id.empty() && !warehouseId.empty() && !stopId.empty()) {
            packages.push_back({id, warehouseId, stopId, std::stoi(priorityText)});
        }
    }

    return packages;
}
