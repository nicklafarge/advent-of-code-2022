#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <exception>

#include "aoc/util.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */


enum class Material {air, rock, fallingSand, restingSand};
class Pos{
public:
    int x;
    int y;
    Pos(const std::string& pos, int offset);
    Pos(int xIn, int yIn);
};
Pos::Pos(int xIn, int yIn): x(xIn), y(yIn){}
Pos::Pos(const std::string& pos, int offset) {
    auto xy = aoc::splitString(pos,",");
    x = std::stoi(xy[0]) - offset;
    y = std::stoi(xy[1]);
}
bool operator== (const Pos& pos1, const Pos& pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}
bool operator!= (const Pos& pos1, const Pos& pos2) {
    return !(pos1==pos2);
}

char mat2char(Material mat) {
    switch(mat) {
        case Material::air:
            return '.';
        case Material::rock:
            return '#';
        case Material::fallingSand:
            return '+';
        case Material::restingSand:
            return 'o';
    }
}
auto mat2color(Material mat) {
    switch(mat) {
        case Material::air:
            return WHITE;
        case Material::rock:
            return CYAN;
        case Material::fallingSand:
            return YELLOW;
        case Material::restingSand:
            return RED;
    }
}
void showGrid(std::vector<std::vector<Material>> grid) {
    for(int i=0; i<grid[0].size(); ++i) {
        for (int j=0; j<grid.size(); ++j) {
            Material mat = grid[j][i];
            std::cout<<mat2color(mat) <<  mat2char(mat) << RESET;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

auto simulateSand(std::vector<std::vector<Material>> grid, Pos source, bool spillover=true) {

    int nRows = static_cast<int>(grid.size());
    int nCols = static_cast<int>(grid[0].size());
    bool onGrid = true;
    int maxUnits = 200000;
    int nUnits = 0;
    while (onGrid && nUnits < maxUnits) {

        // Reached source
        if (grid[source.x][source.y] == Material::restingSand) {
            onGrid = false;
            break;
        }

        Pos sandPos = source;
        Material matStatus = Material::fallingSand;
        grid[source.x][source.y] = matStatus;
        while (onGrid && matStatus != Material::restingSand) {

            bool yOk = sandPos.y+1 < nCols;
            bool xOk = sandPos.x-1 >= 0 && sandPos.x+1 < nRows;

            // Move down one
            if(yOk && grid[sandPos.x][sandPos.y+1] == Material::air) {
                grid[sandPos.x][sandPos.y] = Material::air;
                sandPos.y += 1;
            }
            // Move down/left one
            else if (xOk && yOk && grid[sandPos.x-1][sandPos.y+1] == Material::air) {
                grid[sandPos.x][sandPos.y] = Material::air;
                sandPos.x -= 1;
                sandPos.y += 1;
            }
            // Move down/right one
            else if (xOk && yOk && grid[sandPos.x+1][sandPos.y+1] == Material::air) {
                grid[sandPos.x][sandPos.y] = Material::air;
                sandPos.x += 1;
                sandPos.y += 1;
            }
            // Spill over side (part 1)
            else if(spillover && (!xOk || !yOk)) {
                onGrid = false;
            }
            // Sand at rest
            else {
                matStatus = Material::restingSand;
            }
            if (onGrid)
                grid[sandPos.x][sandPos.y] = matStatus;

        }
        if (onGrid)
            nUnits++;
    }
//    showGrid(grid);
    return nUnits;

}
auto runSimulation(bool sample) {
    std::vector<std::string> all_lines = aoc::readLines(14, sample);

    // Get dimensions
    int xmin=100000, xmax=0, ymin=0, ymax=0;
    for(const std::string& line: all_lines) {
        auto split = aoc::splitString(line, " -> ");
        for (const std::string& coord : split) {
            Pos pos(coord, 0);
            xmin = std::min(xmin, pos.x);
            xmax = std::max(xmax, pos.x);
            ymax = std::max(ymax, pos.y);
        }
    }
    int nRows = xmax-xmin+1;
    int nCols = ymax-ymin+1;
    std::vector<std::vector<Material>> grid(nRows, std::vector<Material>(nCols, Material::air));

    // Construct grid of rocks
    for(const std::string& line: all_lines) {
        auto split = aoc::splitString(line, " -> ");
        Pos current(split[0], xmin);
        for(int i=1; i<split.size(); ++i) {
            Pos next(split[i], xmin);
            if (current.y == next.y) {
                for(int xloc=std::min(current.x, next.x); xloc <= std::max(current.x, next.x); ++xloc) {
                    grid[xloc][current.y] = Material::rock;
                }
            } else if (current.x == next.x) {
                for(int yloc=std::min(current.y, next.y); yloc <= std::max(current.y, next.y); ++yloc) {
                    grid[current.x][yloc] = Material::rock;
                }
            } else{
                std::cout << "Warning: current and next don't match in x or y!" <<std::endl;
            }
            current = next;
        }
    }
    const Pos source("500,0", xmin);

    // Simulate/output part 1
    int nUnitsPt1 = simulateSand(grid, source, true);
    std::cout << "Part 1:" << nUnitsPt1 << std::endl;

    // Add padding to either side of grid (could probably be done smarter....)
    int xpad = 5000;
    std::vector<std::vector<Material>> grid2(nRows+(2*xpad),
                                             std::vector<Material>(nCols+2, Material::air));
    // Copy grid-> grid2
    for(int i=0; i<nRows; i++) {
        for(int j=0; j<nCols; j++) {
            grid2[i+xpad][j] = grid[i][j];
        }
    }
    // Add bottom layer of rock
    for(auto &row : grid2){
        row[nCols+1] = Material::rock;
    }
    // Update source with xpad
    const Pos pt2Source(source.x + xpad, source.y);

    // Simulate/output part 2
    int nUnitsPt2 = simulateSand(grid2, pt2Source, false);
    std::cout << "Part 2:" << nUnitsPt2 << std::endl;

}


int main() {
    runSimulation(true);
    runSimulation(false);
}