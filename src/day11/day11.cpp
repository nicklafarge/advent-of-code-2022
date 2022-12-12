#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

#include <vector>
#include <map>
#include <string>

#include <functional> // for lambda function creation
#include <cmath> // For floor function

#include "aoc/util.hpp"



class Item {

};

class Monkey {
public:
    int ID;
    std::vector<unsigned long> Items;
    std::function<unsigned long(unsigned long)> Operation;
    int TestDiv;
    int TestTrueId;
    int TestFalseId;
    std::function<unsigned long(unsigned long)> Relief;
    unsigned long InspectionCount;

    Monkey(int id, std::vector<unsigned long> items, std::function<unsigned long(unsigned long)> operation,
           int testDiv, int testTrueId, int testFalseId, std::function<unsigned long(unsigned long)> relief)
           : ID(id), Items(std::move(items)), Operation(std::move(operation)), TestDiv(testDiv),
           TestTrueId(testTrueId), TestFalseId(testFalseId), Relief(std::move(relief)),
           InspectionCount(0) {};

    bool Test(int x) const {
        return (x % TestDiv) == 0;
    }

    std::vector<int> throwTo() {
        std::vector<int> monkeyIds;
        for (auto& worryLevel : Items) {
            // Monkey inspects the item
            auto newVal = Operation(worryLevel);
            InspectionCount++;
            // Relief, monkey didn't damage it
            newVal = Relief(newVal);

            // Update the value
            worryLevel = newVal;

            // Test based on the worry level
            int monkeyId = Test(worryLevel) ? TestTrueId : TestFalseId;

            // Say the monkey is throwing this item to
            monkeyIds.emplace_back(monkeyId);
        }
        return monkeyIds;
    }
};
void runSimPart(bool sample, bool partOne) {
    std::vector<std::string> all_lines = aoc::readLines(11, sample);

    std::vector<Monkey> monkeys;
    int monkeyIdx = 0;
    while(monkeyIdx < all_lines.size()) {
        // Monkey ID
        auto idPointerStart = all_lines[monkeyIdx].begin()+7;
        auto idPointerEnd = all_lines[monkeyIdx].end()-1;
        int monkeyId = std::stoi(std::string(idPointerStart, idPointerEnd));

        // Starting Items
        int splitIdx = all_lines[monkeyIdx+1].find(": ", 2);
        std::string startingItemsStr = std::string(all_lines[monkeyIdx+1].begin()+splitIdx+2,
                                                all_lines[monkeyIdx+1].end());
        std::vector<std::string> strList = aoc::splitString(startingItemsStr, ", ");
        std::vector<unsigned long> startingItems;
        std::for_each(strList.begin(), strList.end(),
                      [&startingItems](const std::string& s) {startingItems.push_back(std::stoi(s));
        });

        // Operation
        int opSplitIdx = all_lines[monkeyIdx+2].find(": ", 2);
        std::string operationStr = std::string(all_lines[monkeyIdx+2].begin()+opSplitIdx+2,
                                                   all_lines[monkeyIdx+2].end());
        std::string rhs = std::string(operationStr.begin()+6,operationStr.end());
        char rhsOperator = rhs[4];
        std::string rhsOperatorVar = std::string(rhs.begin()+6, rhs.end());
        std::function<unsigned long(unsigned long)> monkeyOp = [rhsOperator, rhsOperatorVar](unsigned long old) {
            unsigned long opVar = (rhsOperatorVar == "old") ? old : stoi(rhsOperatorVar);
            switch(rhsOperator) {
                case '+':
                    return old + opVar;
                case '-':
                    return old - opVar;
                case '*':
                    return old * opVar;
                case '/':
                    return old / opVar;
                default:
                    throw(std::runtime_error("unknown operator"));

            }
        };

        // Test
        int testSplitIx = all_lines[monkeyIdx+3].find("by ", 2);
        int testDiv = std::stoi(std::string(all_lines[monkeyIdx+3].begin()+testSplitIx+3,
                                               all_lines[monkeyIdx+3].end()));


        int trueSplitIx = all_lines[monkeyIdx+4].find("monkey ", 2);
        int trueVal = std::stoi(std::string(all_lines[monkeyIdx+4].begin()+trueSplitIx+7,
                                            all_lines[monkeyIdx+4].end()));
        int falseSplitIx = all_lines[monkeyIdx+5].find("monkey ", 2);
        int falseVal = std::stoi(std::string(all_lines[monkeyIdx+5].begin()+falseSplitIx+7,
                                            all_lines[monkeyIdx+5].end()));

        // Relief Function
        std::function<unsigned long(unsigned long)> relief;
        if(partOne) {
            relief = [](unsigned long old) {return std::floor(old/3); };
        } else {
            relief = [](unsigned long old) {return old; /* no op */ };
        }

        Monkey monkey {monkeyId, startingItems, monkeyOp, testDiv, trueVal, falseVal, relief};
        monkeys.push_back(monkey);
        monkeyIdx+=7;
    }
    if(!partOne) {
        // Get the product of all divisors (to keep worry values in check!)
        int prod = 1;
        for(const Monkey& monkey : monkeys)
            prod *= monkey.TestDiv;

        // Set the relief function
        for(Monkey& monkey : monkeys)
            monkey.Relief =  [&prod](const long old) {return old % prod; };
    }

    int nRounds = partOne ? 20 : 10000;
    for (auto j=0; j<nRounds; ++j) {
        for (Monkey &monkey : monkeys) {
            auto throwTo = monkey.throwTo();
            for (int i=0; i< throwTo.size(); ++i) {
                monkeys[throwTo[i]].Items.push_back(monkey.Items[i]);
            }
            monkey.Items.clear();
        }
    }

//    for(Monkey& monkey : monkeys) {
//        std::cout << "Monkey " << monkey.ID << " inspected items " <<
//            monkey.InspectionCount << " times." << std::endl;
//    }

    // Sort by Inspection Count
    sort(monkeys.begin(), monkeys.end(),[](const Monkey& m1, const Monkey& m2){
        return m1.InspectionCount > m2.InspectionCount;
    });
    unsigned long monkeyBusiness = monkeys[0].InspectionCount * monkeys[1].InspectionCount;

    // Output
    int partId = partOne ? 1 : 2;
    std::cout << "Monkey Business (" << partId << "): " << monkeyBusiness << std::endl;
}

void runSimulation(bool sample) {
    runSimPart(sample, true);
    runSimPart(sample, false);
}

int main() {
    runSimulation(true);
    runSimulation(false);
}