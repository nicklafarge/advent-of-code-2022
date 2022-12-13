#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

#include <vector>
#include <map>
#include <string>


#include "aoc/util.hpp"


struct Pos {

    int x;
    int y;

    Pos operator + (Pos const &obj) const {
        return Pos {x + obj.x, y + obj.y};
    }
    Pos operator - (Pos const &obj) const {
        return Pos {x - obj.x, y - obj.y};
    }
    bool operator == (Pos const &obj) const {
        return (x==obj.x && y==obj.y);
    }
    bool operator != (Pos const &obj) const {
        return (x!=obj.x || y!=obj.y);
    }

    bool isTouching(Pos other) const{
        return abs(other.x - x)<=1 && abs(other.y - y)<=1;
    }

    void print() const {
        std::cout << "[" << x << "," << y << "]" << std::endl;
    }

};

auto runSimulation(int which, int nFollow) {

    auto test = std::__fs::filesystem::current_path();
    std::string filename;
    switch(which){
        case 1:
            filename = "res/day9_sample.txt";
            break;
        case 2:
            filename = "res/day9_larger.txt";
            break;
        default:
            filename = "res/day9.txt";
    }
    std::vector<std::string> all_lines = aoc::readLines(filename);

    std::vector<std::vector<unsigned int>> grid {};
    std::map<char, Pos> stepMap = {
        {'R', Pos {0, 1}},
        {'L', Pos {0, -1}},
        {'U', Pos {1, 0}},
        {'D', Pos {-1, 0}},
        };
    Pos start = {0, 0};

    // Assume [0] is head and [back] is tail
    std::vector<Pos> snake {Pos{0, 0}};
    for (auto i=0; i<nFollow; ++i)
        snake.push_back(Pos {0, 0});

    // Vector of visited_ states by the tail
    std::vector<Pos> visited;
    visited.push_back(snake.back());

    bool printGrid = false;
    for (const auto &l : all_lines) {
        // Parse string
        Pos step = stepMap[l[0]];
        int nSteps = std::stoi(std::string(l.begin()+2, l.end()));

        // Iterate through each step
        for(unsigned int i=0; i<nSteps; ++i) {

            // Update head state and head/tail difference
            snake[0] = snake[0] + step;


            // Update followers
            for(auto k=1; k<=nFollow; ++k) {
                Pos leader = snake[k-1];
                Pos follower = snake[k];
                Pos htDist = leader - follower;

                // Update tail position if needed
                if(!follower.isTouching(leader)) {
                    Pos followStep{};
                    if(htDist.x==0 || htDist.y ==0) {
                        followStep = Pos {htDist.x/2, htDist.y/2};
                    } else {
                        followStep = Pos {((htDist.x > 0) - (htDist.x < 0)),
                                          ((htDist.y > 0) - (htDist.y < 0))};
                    }
                    snake[k] = follower + followStep;
                }
            }

            // See if we have been here before
            bool isNewTailPos = true;
            for (Pos previous : visited) {
                if(previous == snake.back()){
                    isNewTailPos = false;
                    break;
                }
            }

            // Ad the new tail to the visited_ list if needed
            if(isNewTailPos)
                visited.push_back(snake.back());


            // For Debugging...
            if (printGrid) {
                for(int j=0; j<14; ++j) {
                    for(int k=0; k<14; ++k) {
                        Pos visPos = {6-j, k-6};

                        bool isPrevVisited = false;
                        for (Pos previous : visited) {
                            if(previous == visPos){
                                isPrevVisited = true;
                                break;
                            }
                        }
                        std::string symbol = ".";
                        if(visPos==snake[0]) {
                            symbol="H";
                        }
                        for(auto kv=1; kv<=nFollow; ++kv) {
                            if(visPos==snake[kv]) {
                                symbol = std::to_string(kv);
                                break;
                            }
                        }
                        if(symbol=="." && visPos==start){
                            symbol = "s";
                        } else if (symbol=="." && isPrevVisited) {
                            symbol = "#";
                        }
                        std::cout << symbol;
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
    std::cout << "Visited: " << visited.size() << std::endl;
    return visited;
}



int main() {
    runSimulation(1, 1);
    runSimulation(3, 1);

    runSimulation(1, 9);
    runSimulation(2, 9);
    runSimulation(3, 9);
}