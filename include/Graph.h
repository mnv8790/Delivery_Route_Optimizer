#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <utility>
#include <vector>

class Graph {
private:
    std::unordered_map<int, std::vector<std::pair<int, double>>> adj;

public:
    void addEdge(int from, int to, double distance);
    const std::vector<std::pair<int, double>>& getNeighbors(int node) const;
    std::vector<int> getAllNodes() const;
    std::unordered_map<int, double> shortestPaths(int source) const;
};

#endif
