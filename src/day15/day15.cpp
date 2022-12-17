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

#include "aoc/util.hpp"

class Pos{
public:
    int x, y;
    explicit Pos(const std::string& inputStr, int xOffset, int yOffset);
    Pos(int xIn, int yIn);
};
Pos::Pos(int xIn, int yIn): x(xIn), y(yIn){}
Pos::Pos(const std::string& inputStr, int xOffset, int yOffset) {
    auto xStart = inputStr.find("x=")+2;
    auto xEnd = inputStr.find(',', xStart);
    auto xStr = inputStr.substr(xStart, xEnd-xStart);

    auto yStart = inputStr.find("y=")+2;
    auto yEnd = inputStr.size();
    auto yStr = inputStr.substr(yStart, yEnd-yStart);

    x = std::stoi(xStr) + xOffset;
    y = std::stoi(yStr) + yOffset;

}
Pos operator+(const Pos& pos1, const Pos& pos2) {
    return {pos1.x + pos2.x, pos1.y + pos2.y};
}
bool operator== (const Pos& pos1, const Pos& pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}
bool operator!= (const Pos& pos1, const Pos& pos2) {
    return !(pos1==pos2);
}

int manhattanDistance(const Pos& pos1, const Pos& pos2) {
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

struct Range {
    int x0, xf;
};

class ScanResult{
public:
    Pos sensor, beacon;
    int mDist;
    ScanResult(Pos sensorIn, Pos beaconIn):sensor(sensorIn), beacon(beaconIn) {
        mDist = manhattanDistance(sensor, beacon);
    }
};

typedef std::map<int, std::vector<Range>> RowRangeVec;
enum class DuplicateSearchResult {NewEntry, ContainedCompletely};

DuplicateSearchResult findDuplicates(std::vector<Range> &searchedPoints, Range newRange, Range& rangeToAdd) {

    for (int i=0; i<searchedPoints.size(); ++i) {

        // Separate ranges
        Range exploredRange = searchedPoints[i];
        if (newRange.xf < exploredRange.x0 || exploredRange.xf < newRange.x0)
            continue;

        // Stop if this is entirely contained in the entry
        if (newRange.x0 >= exploredRange.x0 && newRange.xf <= exploredRange.xf)
            return DuplicateSearchResult::ContainedCompletely;

        // Combine overlapping ranges
        int newMin = std::min(newRange.x0, exploredRange.x0 );
        int newMax = std::max(newRange.xf, exploredRange.xf);
        Range combinedRangeToAdd {newMin, newMax};

        // Remove the duplicate
        searchedPoints.erase(searchedPoints.begin() + i);

        // Recursion call to find other pre-existing overlapping ranges
        return findDuplicates(searchedPoints, combinedRangeToAdd, rangeToAdd);
    }
    // Set the range to be added
    rangeToAdd = newRange;
    return DuplicateSearchResult::NewEntry;
}
void searchArea(RowRangeVec &searchedPoints, const Pos& sPos,
                int beaconManhattanDist, int searchCol) {

    // Projection distance to search axis
    int distanceFrom = searchCol - sPos.y;

    // Search column is too far away
    if (abs(distanceFrom) > beaconManhattanDist)
        return;

    // Project directly to the search column (moving along the y-axis)
    Pos projToRow = sPos + Pos(0, distanceFrom);

    // Range to add along the specific column
    int distRemaining = beaconManhattanDist - abs(distanceFrom);

    // Create the new range
    Range newRange {projToRow.x -distRemaining, projToRow.x + distRemaining};

    // Iterate through explored ranges to see if this "new" one overlaps any existing ones
    std::vector<Range>& exploredRanges = searchedPoints[searchCol];
    Range rangeToAdd{}; // Set by findDuplicates function
    DuplicateSearchResult searchResult = findDuplicates(exploredRanges, newRange, rangeToAdd);
    if (searchResult == DuplicateSearchResult::NewEntry) {
        exploredRanges.push_back(rangeToAdd);
    }

}


auto runSimulation(bool sample) {

    // Initialize simulation
    std::vector<std::string> all_lines = aoc::readLines(15, sample);
    int searchCol = sample ? 10 : 2000000;
    RowRangeVec searchedPoints;

    // Read results from puzzle input
    std::vector<ScanResult> scanResults;
    for (const std::string& line : all_lines) {
        std::cout<<line << std::endl;
        auto split = aoc::splitString(line, ": ");
        Pos sensor(split[0], 0, 0);
        Pos beacon(split[1], 0, 0);
        scanResults.emplace_back(sensor, beacon);
    }

    // Find the results for the specific column (Part 1)
    for (const ScanResult& sr : scanResults) {
        searchArea(searchedPoints, sr.sensor, sr.mDist, searchCol);
    }

    // Count all the searched points along searchCol
    int count = 0;
    for(auto range : searchedPoints[searchCol]) {
        count+=(range.xf - range.x0);
    }
    std::cout << "Count (1): "<< count << std::endl;

    // Specify bounds (part 2)
    int xmin = 0;
    int ymin = 0;
    int xmax = sample ? 20 : 4000000;
    int ymax = sample ? 20 : 4000000;

    // Now search through all valid columns (takes a while!)
    for (int yval=ymin; yval < ymax; ++yval) {

        if (yval % 100000 == 0)
            std::cout << yval << std::endl;

        for (const ScanResult& sr : scanResults) {
            searchArea(searchedPoints, sr.sensor, sr.mDist, yval);
        }
        // Clip the bounds to be within the ranges (a bit inefficient but oh well...)
        for(Range& range : searchedPoints[yval]) {
            range.x0 = std::max(xmin, range.x0);
            range.xf = std::min(range.xf,xmax);
        }
    }
    // Find the column where multiple ranges exist(implies that there is a gap of one
    int coly=-1;
    for (int yval=ymin; yval < ymax; ++yval) {
        if(searchedPoints[yval].size() > 1) {
            coly = yval;
            break;
        }
    }

    // Now find the row as the one missing element in the two ranges
    auto row = searchedPoints[coly];
    int xf = (row[1].x0 > row[0].xf) ? row[0].xf : row[1].xf;
    int rowx = xf + 1;
    long frequency = rowx * 4000000l + coly; // Careful! integers lose precision here.
    std::cout << "Frequency (2): "<< frequency << std::endl;
}


int main() {
    runSimulation(true);
    runSimulation(false);
}