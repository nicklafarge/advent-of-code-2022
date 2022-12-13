#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

#include <vector>
#include <cmath>

#include "aoc/util.hpp"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"      /* Yellow */
#define WHITE   "\033[37m"      /* White */

///@brief A graph data structure to search for the shortest parth
class Graph {

public:
    Graph(int rows, int cols, int end, std::vector<std::vector<int>> heightGrid)
    : gridRows_(rows) , gridCols_(cols) , end_(end)
    , numVertices_(gridRows_ * gridCols_)
    , adjLists_(numVertices_, std::vector<int> {})
    , visited_(), heights_(std::move(heightGrid))
    {}

    ///@brief add a connection from src->dest in the graph
    void addEdge(int src, int dest);

    ///@brief breadth first search, with terminal criteria controlled by partOne bool
    auto BFS(int start, bool partOne, bool showPath);

    // Helper functions to convert between (x,y) and id=(x*nRows+y)
    int rc2id(int x, int y);
    std::vector<int> id2rc(int id);
private:
    // Private member variables
    int gridRows_;
    int gridCols_;
    int numVertices_;
    int end_;
    std::vector<std::vector<int>> adjLists_;
    std::vector<bool> visited_;
    std::vector<std::vector<int>> heights_;
};

int Graph::rc2id(int x, int y) {
    return (x * gridCols_) + y;
}
std::vector<int> Graph::id2rc(int id) {
    int x = std::floor(id / gridCols_);
    int y = id % gridCols_;
    return std::vector<int> {x, y};
}

void Graph::addEdge(int src, int dest) {
    // Add this edge if it doesn't exist already
    if (std::count(adjLists_[src].begin(), adjLists_[src].end(), dest) == 0)
        adjLists_[src].push_back(dest);
}
auto Graph::BFS(int start, bool partOne, bool showPath) {

    // So we don't visit the same node twice
    visited_ = std::vector<bool>(numVertices_, false);

    // To keep track of the order (back out the shortest path once we
    // encounter the terminal criteria
    std::vector<int> previous(numVertices_);

    // Queue for performing the breadth-first search
    std::deque<int> queue {};

    // Initialize at the starting node
    visited_[start] = true;
    queue.push_back(start);

    // The order in which the nodes were encountered
    std::vector<int> visitedOrder {};

    // Iterate through queue, adding new vertices as we go
    while (!queue.empty()) {

        // Get first element from queue
        int currVertex = queue.front();
        auto currXY = id2rc(currVertex);
        visitedOrder.push_back(currVertex);
        queue.pop_front();

        // Shortest path found!
        if ((partOne && currVertex == end_) ||
            !partOne && heights_[currXY[0]][currXY[1]] == 0) {
            break;
        }

        // Add the connected vertices to the queue
        for (auto adjVertex : adjLists_[currVertex]) {

            // Only added if they haven't been visited yet
            if (!visited_[adjVertex]) {
                visited_[adjVertex] = true;
                previous[adjVertex] = currVertex;
                queue.push_back(adjVertex);
            }
        }
    }

    // Find path from Start to End
    std::vector<int> startEndPath = {};
    int nextBack = visitedOrder.back();
    while (nextBack != start) {
        startEndPath.insert(startEndPath.begin(), nextBack);
        nextBack = previous[nextBack];
    }

    // Optionally "plot" the shortest path in the terminal
    if (showPath) {
        std::vector<char> printGrid(numVertices_, '.');

        // Get the symbol to print
        for (int i = 0; i < startEndPath.size() - 1; ++i) {
            int now = startEndPath[i];
            int next = startEndPath[i + 1];
            if (now == next - 1) {          // moving right
                printGrid[now] = '>';
            } else if (now == next + 1) {   // moving left
                printGrid[now] = '<';
            } else if (now < next) {        // moving down
                printGrid[now] = 'v';
            } else if (now > next) {        // moving up
                printGrid[now] = '^';
            }
        }
        if(partOne) // Add an S for part one (doesn't really make sense for part 2)
            printGrid[start] = 'S';

        // For showing the end state
        printGrid[end_] = 'E';

        // Print them to the terminal
        std::cout << std::endl;
        for (int i = 0; i < gridRows_; ++i) {
            for (int j = 0; j < gridCols_; ++j) {
                int id = i * gridCols_ + j;
                auto color = printGrid[id] != '.' ? YELLOW : WHITE;
                std::cout << color << printGrid[id] << RESET;
            }
            std::cout << std::endl;
        }
    }

    return startEndPath;
}


auto runSimulation(bool sample) {
    // Read in lines
    std::vector<std::string> all_lines = aoc::readLines(12, sample);

    // Row and columns
    int nRows = static_cast<int>(all_lines.size());
    int nCols = static_cast<int>(all_lines[0].size());

    // Keep track of all the heights_
    std::vector<std::vector<int>> heights(nRows, std::vector<int>(nCols));

    // Iterate through input to store all the heights, start, and end states
    int start, end, height;

    // Go row-by-row through puzzle input
    for(int i=0; i< nRows; ++i) {
        std::string line = all_lines[i];

        // Iterate through each column
        for (int j = 0; j < nCols; ++j) {
            // compute ID (to save start/end)
            int id = i*nCols + j;

            // Set the height based on the given identifier
            char identifier = line[j];
            switch(identifier) {
                case 'S':
                    height = 0;
                    start = id;
                    break;
                case 'E':
                    height = 25;
                    end = id;
                    break;
                default:
                    height = identifier - 'a';
                    break;
            }
            heights[i][j] = height;
        }
    }
    // Create graph
    Graph g {nRows, nCols, end, heights};

    // Add the edges between nodes based on height
    for(int i=0; i< nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            // Get ID/height
            int id = g.rc2id(i, j);
            int h = heights[i][j];

            // Connection up
            if(i > 0 && heights[i-1][j] <= h+1) {
                g.addEdge(id, (i-1)*nCols + j);
            }
            // Connection left
            if(j > 0 && heights[i][j-1] <= h+1) {
                g.addEdge(id, i*nCols + (j-1));
            }
            // Connection down
            if(i < (nRows-1) && heights[i+1][j] <= h+1) {
                g.addEdge(id, (i+1)*nCols + j);
            }
            //Connection right
            if(j < (nCols-1) && heights[i][j+1] <= h+1) {
                g.addEdge(id, i*nCols + (j+1));
            }
        }
    }

    // Breadth-first search to find shortest path from S->E
    bool partOne = true;
    bool plotOutput = true;
    auto visitedOrder1 = g.BFS(start, partOne, plotOutput);
    int nStepsPt1 = static_cast<int>(visitedOrder1.size());
    std::cout << "Shortest path (1): " << nStepsPt1 << std::endl;

    // Part 2 - there is probably a smarter way without recreating the graph but this is easy :)
    Graph g2 {nRows, nCols, end, heights};

    // Add the edges for part 2 (this time going down instead of up)
    for(int i=0; i< nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            int id = i*nCols + j;
            int h = heights[i][j];

            // Connection up
            if(i > 0 && heights[i-1][j] >= h-1) {
                g2.addEdge(id, (i-1)*nCols + j);
            }
            // Connection left
            if(j > 0 && heights[i][j-1] >= h-1) {
                g2.addEdge(id, i*nCols + (j-1));
            }
            // Connection down
            if(i < (nRows-1) && heights[i+1][j] >= h-1) {
                g2.addEdge(id, (i+1)*nCols + j);
            }
            //Connection right
            if(j < (nCols-1) && heights[i][j+1] >= h-1) {
                g2.addEdge(id, i*nCols + (j+1));
            }
        }
    }

    // Breadth first search to find shortest parth from E->a
    partOne = false;
    auto visitedOrder2 = g2.BFS(end, partOne, plotOutput);
    int nStepsPt2 = static_cast<int>(visitedOrder2.size());
    std::cout << "Shortest path (2): " << nStepsPt2 << std::endl;
}

int main() {
    runSimulation(true);
    runSimulation(false);
}