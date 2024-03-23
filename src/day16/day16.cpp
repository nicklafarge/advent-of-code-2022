#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <exception>
#include <map>
#include <unordered_map> // for DG
#include <functional> //for std::hash

#include "aoc/util.hpp"

class Valve {
public:
    std::string id;

    bool operator==(const Valve& rhs) const {
        return rhs.id == id;
    }
};

class ValveHashFunction {
public:
    std::size_t operator()(const Valve &valve) const {
        return std::hash<std::string>()(valve.id);
    }
};

struct Edge {
    int weight;
    std::string to;
};

///@brief A graph data structure to search for the shortest parth
class Graph {

public:
    std::unordered_map<Valve, std::vector<Edge>, ValveHashFunction> adj;

    Graph() = default;
    ///@brief add a connection from between valves
    void addEdge(const Valve& src, const Valve& dest, int weight) {
        adj[src].push_back({weight, dest.id});
    }

private:
};


auto runSimulation(bool sample) {

    // Initialize simulation
    std::vector<std::string> all_lines = aoc::readLines(16, sample);
    Graph graph;
    for(const std::string& line : all_lines) {
        // Compute ID
        std::string id = line.substr(6,2);

        // Compute Flow Rate
        auto loc1=  line.find("rate=");
        auto loc2 = line.find(';');
        auto weightStart = loc1+5;
        int flowRate = std::stoi(line.substr(weightStart, loc2-weightStart));

        // Source valve
        Valve offValve {id};
        Valve onValve {id+"_on"};

        // Add connection for going from off to on
        graph.addEdge(offValve, onValve, flowRate);

        // Compute destinations
        auto to_split1 = aoc::splitString(line, "valve");
        auto destIds = aoc::splitString(to_split1[1], ", ");
        for (int i=1; i< destIds.size(); ++i){
            Valve dest {destIds[i]};
            graph.addEdge(offValve, dest, 0);
            graph.addEdge(onValve, dest, 0);
        }
        auto x = 123;
    }
}


int main() {
    runSimulation(true);
    runSimulation(false);
}