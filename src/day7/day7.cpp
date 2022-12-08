#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include "aoc/util.hpp"


struct File {
    std::string name;
    unsigned int size;
};

class Dir {
public:
    std::string name;
    std::shared_ptr<Dir> parent;
    std::vector<std::shared_ptr<Dir>> subdirs;
    std::vector<File> files;
    unsigned int size;
    explicit Dir(std::string dirName, std::shared_ptr<Dir> parent)
    : name(std::move(dirName)), parent(std::move(parent)), subdirs(), files(), size(0) {}
    explicit Dir(std::string dirName, std::shared_ptr<Dir> parent, std::vector<std::shared_ptr<Dir>> subdirs, std::vector<File> files)
    : name(std::move(dirName)), parent(std::move(parent)), subdirs(std::move(subdirs)), files(std::move(files)), size(0) {}
};

void findPart1Files(const std::shared_ptr<Dir> &cwd, int partOneLimit,
                    const std::shared_ptr<std::vector<unsigned int>>& foundFiles) {
    // Base Case
    if(!cwd) {
        return;
    }

    if (cwd->name != "/" && cwd->size <= partOneLimit) {
        foundFiles->push_back(cwd->size);
    }

    for (const auto& subdir : cwd->subdirs) {
        findPart1Files(subdir, partOneLimit, foundFiles);
    }
}

void findPart2Files(const std::shared_ptr<Dir> &cwd, unsigned int partTwoLimit,
                    const std::shared_ptr<std::vector<unsigned int>>& foundFiles) {
    // Base Case
    if(!cwd) {
        return;
    }

    if (cwd->size >= partTwoLimit) {
        foundFiles->push_back(cwd->size);
    }

    for (const auto& subdir : cwd->subdirs) {
        findPart2Files(subdir, partTwoLimit, foundFiles);
    }
}

auto runSimulation(bool sample) {
    std::vector<std::string> all_lines = aoc::readLines(7, sample);
    std::shared_ptr<Dir> cwd;
    bool ls_active = false;
    std::shared_ptr<Dir> root = std::make_shared<Dir>("/", nullptr);

    // Fill out file tree
    for (auto &l : all_lines) {
        // Command
        if (l[0] == '$') {
            ls_active = false;

            std::string command = std::string(l.begin()+2, l.begin()+4);
            if(command == "cd") {
                std::string dirName = std::string(l.begin()+5, l.end());

                // Root case
                if (dirName == "/") {
                    cwd = root;
                    continue;
                }
                // Navigate to subdirectory
                for (const auto& subdir : cwd->subdirs) {
                    if (subdir->name == dirName) {
                        cwd = subdir;
                        continue;
                    }
                }
                // Move up one level
                if (dirName == "..") {
                    cwd = cwd->parent;
                    continue;
                }
            } else if(command == "ls") {
                ls_active = true;
            }
        }
        // Contents from ls
        else if (ls_active) {
            auto splitString = l.substr(0, l.find(" "));
            if (splitString == "dir") {
                std::string dirName = std::string(l.begin()+4, l.end());
                std::shared_ptr<Dir> newDir = std::make_shared<Dir>(dirName, cwd);
                cwd->subdirs.push_back(newDir);
            } else {
                unsigned int fileSize = std::stoi(splitString);
                std::string fileName = l.substr(splitString.size(), -1);
                cwd->files.push_back(File {fileName, fileSize});
                cwd->size += fileSize;

                // Increase size all the way up to root
                auto fileSizeDir = std::make_shared<Dir>(*cwd);
                while (fileSizeDir->parent) {
                    fileSizeDir->parent->size += fileSize;
                    fileSizeDir = fileSizeDir->parent;
                }
            }
        }
    }

    cwd = root;

    // For part 1 data
    int part1SizeLimit = 100000;
    auto foundFiles = std::make_shared<std::vector<unsigned int>>();
    findPart1Files(root, part1SizeLimit, foundFiles);
    unsigned int sumOfElements = 0;
    for (int i=0; i<foundFiles->size(); i++) {
        sumOfElements += (*foundFiles)[i];
    }
    std::cout << "Part 1 sum: " << sumOfElements << std::endl;

    unsigned int totalAvailable = 70000000;
    unsigned int neededUnused = 30000000;

    unsigned int totalUsed = root->size;
    unsigned int needToFreeUp = neededUnused - (totalAvailable - totalUsed);
    auto foundFilesPt2 = std::make_shared<std::vector<unsigned int>>();
    findPart2Files(root, needToFreeUp, foundFilesPt2);

    unsigned int min = *min_element((*foundFilesPt2).begin(), (*foundFilesPt2).end());
    std::cout << "Part 2: " << min << std::endl;
    return sumOfElements;
}



int main() {
    auto dir = runSimulation(true);
    runSimulation(false);
}
