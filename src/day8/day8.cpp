#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include "aoc/util.hpp"

auto isVisible(std::vector<std::vector<unsigned int>> grid, unsigned int treeRow, unsigned int treeCol) {
    std::vector<bool> visible = {true, true, true, true};
    unsigned int height = grid[treeRow][treeCol];

    // Top
    for (unsigned int i=0; i < treeRow; ++i) {
        unsigned int tree = grid[i][treeCol];
        if (tree >= height) {
            visible[0] = false;
            break;
        }
    }

    // Left
    for (unsigned int i=0; i < treeCol; ++i) {
        unsigned int tree = grid[treeRow][i];
        if (tree >= height) {
            visible[1] = false;
            break;
        }
    }

    // Bottom
    for (unsigned int i=treeRow+1; i < grid.size(); ++i) {
        unsigned int tree = grid[i][treeCol];
        if (tree >= height) {
            visible[2] = false;
            break;
        }
    }

    // Right
    for (unsigned int i=treeCol+1; i < grid[0].size(); ++i) {
        unsigned int tree = grid[treeRow][i];
        if (tree >= height) {
            visible[3] = false;
            break;
        }
    }

    bool anyVisible = false;
    for (auto v : visible)
        if (v) {
            anyVisible = true;
            break;
        }

    return anyVisible;
}

auto viewingDistanceScore(std::vector<std::vector<unsigned int>> grid, unsigned int treeRow, int treeCol) {
    std::vector<unsigned int> viewingDistances = {0, 0, 0, 0};
    unsigned int height = grid[treeRow][treeCol];

    // Top
    for (int i=treeRow-1; i >= 0; i--) {
        unsigned int tree = grid[i][treeCol];
        if (tree >= height) {
            viewingDistances[0] = treeRow - i;
            break;
        }
        viewingDistances[0] = treeRow;
    }

    // Left
    for (int i=treeCol-1; i >= 0; i--) {
        unsigned int tree = grid[treeRow][i];
        if (tree >= height) {
            viewingDistances[1] = treeCol - i;
            break;
        }
        viewingDistances[1] = treeCol;
    }

    // Bottom
    for (unsigned int i=treeRow+1; i < grid.size(); ++i) {
        unsigned int tree = grid[i][treeCol];
        if (tree >= height) {
            viewingDistances[2] = i-treeRow;
            break;
        }
        viewingDistances[2] = (grid.size()-1) - treeRow;
    }

    // Right
    for (unsigned int i=treeCol+1; i < grid[0].size(); ++i) {
        unsigned int tree = grid[treeRow][i];
        if (tree >= height) {
            viewingDistances[3] = i-treeCol;
            break;
        }
        viewingDistances[3] = (grid[0].size()-1) - treeCol;
    }

    unsigned int score = 1;
    for (auto v : viewingDistances)
        score *= v;

    return score;
}

auto runSimulation(bool sample) {
    std::vector<std::string> all_lines = aoc::readLines(8, sample);

    std::vector<std::vector<unsigned int>> treeGrid {};
    for (const auto &l : all_lines) {
        std::vector<unsigned int> row;
        for (char x : l) {
            row.push_back(x - '0');
        }
        treeGrid.push_back(row);
    }

    // Part 1
    unsigned int visibleCount = 0;
    for (auto i=0; i<treeGrid.size(); i++){
        for (auto j=0; j<treeGrid[1].size(); j++){
            bool visible = isVisible(treeGrid, i, j);
            if (visible)
                visibleCount++;
        }
    }

    //Part 2
    unsigned int bestScore = 0;
    for (auto i=0; i<treeGrid.size(); i++){
        for (auto j=0; j<treeGrid[1].size(); j++){
            unsigned int score = viewingDistanceScore(treeGrid, i, j);
            if (score > bestScore)
                bestScore = score;
        }
    }
    std::cout << "Part 1: " << visibleCount << std::endl;
    std::cout << "Part 2: " << bestScore << std::endl;
    return 0;
}



int main() {
    auto dir = runSimulation(true);
    runSimulation(false);
}
