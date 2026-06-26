#include "Graph.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>

void Graph::addEdge(int from, int to, double distance) {
    adj[from].push_back({to, distance});
    // Treating roads as two-way roads for this small dataset.
    adj[to].push_back({from, distance});
}

const std::vector<std::pair<int, double>>& Graph::getNeighbors(int node) const {
    static const std::vector<std::pair<int, double>> emptyList;

    auto it = adj.find(node);
    if (it == adj.end()) {
        return emptyList;
    }

    return it->second;
}

std::vector<int> Graph::getAllNodes() const {
    std::vector<int> nodes;

    for (const auto& item : adj) {
        nodes.push_back(item.first);
    }

    std::sort(nodes.begin(), nodes.end());
    return nodes;
}

std::unordered_map<int, double> Graph::shortestPaths(int source) const {
    const double INF = std::numeric_limits<double>::max();
    std::unordered_map<int, double> distance;

    for (int node : getAllNodes()) {
        distance[node] = INF;
    }

    distance[source] = 0.0;

    // Dijkstra: pick the nearest unprocessed node using a min priority queue.
    std::priority_queue<
        std::pair<double, int>,
        std::vector<std::pair<double, int>>,
        std::greater<std::pair<double, int>>
    > pq;

    pq.push({0.0, source});

    while (!pq.empty()) {
        double currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDistance > distance[currentNode]) {
            // Means that a better path was already found earlier.
            continue;
        }

        for (const auto& next : getNeighbors(currentNode)) {
            int nextNode = next.first;
            double roadDistance = next.second;

            if (distance[currentNode] + roadDistance < distance[nextNode]) {
                distance[nextNode] = distance[currentNode] + roadDistance;
                pq.push({distance[nextNode], nextNode});
            }
        }
    }

    return distance;
}
