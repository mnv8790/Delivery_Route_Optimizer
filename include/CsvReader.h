#ifndef CSV_READER_H
#define CSV_READER_H

#include "Graph.h"

#include <string>
#include <vector>

struct Warehouse {
    std::string warehouse_id;
    int node_id;
};

struct Stop {
    std::string stop_id;
    int node_id;
};

struct Vehicle {
    std::string vehicle_id;
    std::string warehouse_id;
    int capacity;
    double cost_per_km;
    double max_distance_per_day;
};

struct Package {
    std::string package_id;
    std::string warehouse_id;
    std::string stop_id;
    int priority;
};

class CsvReader {
public:
    static void loadRoads(const std::string& filename, Graph& graph);
    static std::vector<Warehouse> loadWarehouses(const std::string& filename);
    static std::vector<Stop> loadStops(const std::string& filename);
    static std::vector<Vehicle> loadVehicles(const std::string& filename);
    static std::vector<Package> loadPackages(const std::string& filename);
};

#endif
