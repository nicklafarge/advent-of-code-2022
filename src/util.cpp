#include <iostream>
#include <fstream>
#include <filesystem>

#include "aoc/util.hpp"


namespace aoc {

std::vector<std::string> readLines(const std::string& filename) {
    std::fstream newfile (filename,std::ios::in);
    std::vector<std::string> all_lines;
    if (newfile){
        std::string tp;
        while(getline(newfile, tp)){
            all_lines.push_back(tp);
        }
        newfile.close(); //close the file object.
    }
    return all_lines;
}

std::vector<std::string> readLines(int day, bool sample) {
    auto test = std::__fs::filesystem::current_path();
    std::string filename = sample ?
            "res/day" + std::to_string(day) + "_sample.txt" :
            "res/day" + std::to_string(day) + ".txt";
    return readLines(filename);
}


std::vector<std::string> splitString(const std::string& stringSplit,
                                     const std::string& delimiter) {
    size_t pos = 0;
    std::string s = stringSplit;
    std::string token;
    std::vector<std::string> split;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        split.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    split.push_back(s.substr(0, pos));
    return split;
}


}