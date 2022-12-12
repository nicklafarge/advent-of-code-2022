#include <vector>

namespace aoc {
    std::vector<std::string> readLines(const std::string& filename);
    std::vector<std::string> readLines(int day, bool sample);

    std::vector<std::string> splitString(const std::string& stringSplit,
                                         const std::string& delimiter);
}