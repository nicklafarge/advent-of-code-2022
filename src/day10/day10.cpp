#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>


#include "aoc/util.hpp"


auto runSimulation(bool sample) {
    std::vector<std::string> all_lines = aoc::readLines(10, sample);

    std::vector<int> xCycle;
    int x = 1;
    int instructionIndex = 0;
    for (const auto &l : all_lines) {
        auto command = l.substr(0, l.find(" "));
        int nCycles = (command == "addx") ? 2 : 1;
        for (auto i=0; i<nCycles; ++i) {
            xCycle.push_back(x);
        }
        if(command == "addx")
            x += std::stoi(l.substr(command.size(), -1));
    }
    // Last one
    xCycle.push_back(x);

    // Get the important ones
    std::vector<int> importantIndices;
    std::vector<int> importantSignals;
    int importantIdx = 20;
    int part1Result = 0;
    while (importantIdx < xCycle.size()) {
        part1Result += importantIdx * xCycle[importantIdx-1];
        importantSignals.push_back(xCycle[importantIdx]);
        importantIndices.push_back(importantIdx);
        importantIdx += 40;
    }
    std::cout << "Part 1: " << part1Result << std::endl;

    int linei, registerX;
    std::string pixel;
    for(auto i=0; i<xCycle.size(); ++i) {
        linei = i % 40;
        if (linei == 0)
            std::cout << std::endl;

        registerX = xCycle[i];
        if (linei-1 == registerX || linei == registerX || linei+1 == registerX)
            pixel = "#";
        else
            pixel = ".";

        std::cout << pixel;
    }

    return importantSignals;
}



int main() {
    auto xCycle = runSimulation(true);
    runSimulation(false); // Part 2:L EPJBRKAH
}