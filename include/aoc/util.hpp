#include <vector>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

namespace aoc {
    std::vector<std::string> readLines(const std::string& filename);
    std::vector<std::string> readLines(int day, bool sample);

    std::vector<std::string> splitString(const std::string& stringSplit,
                                         const std::string& delimiter);
}