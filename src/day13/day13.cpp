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

enum class CorrectionResult {correct, incorrect, undetermined};

class SignalChar {
public:
    explicit SignalChar(char sChar);
    [[nodiscard]] bool isOpening() const;
    [[nodiscard]] bool isClosing() const;
    [[nodiscard]] bool isComma() const;
    [[nodiscard]] bool isPunctuation() const;
    [[nodiscard]] bool isNumber() const;
    [[nodiscard]] char c() const;
private:
    char sChar_;
};
SignalChar::SignalChar(char sChar): sChar_(sChar) {}

bool SignalChar::isOpening() const {return sChar_ == '[';}
bool SignalChar::isClosing() const {return sChar_ == ']';}
bool SignalChar::isComma() const {return sChar_ == ',';}
bool SignalChar::isPunctuation() const {return (isOpening() || isClosing() || isComma());}
bool SignalChar::isNumber() const {return !isPunctuation();}
char SignalChar::c() const {return sChar_;}



class Packet {
public:
    explicit Packet(std::string packet): packet_(std::move(packet)) {}
    Packet(const Packet& other): Packet(other.packet()) {}
    [[nodiscard]] std::string packet() const { return packet_; }
    [[nodiscard]] int size() const{return static_cast<int>(packet_.size());}
    SignalChar operator[](int i) const {return SignalChar(packet_[i]);}
    void insert(int index, const std::string& value){packet_.insert(index, value);}
    [[nodiscard]] auto begin() const {return packet_.begin();}
    [[nodiscard]] auto end() const {return packet_.end();}
private:
    std::string packet_;
};


int numEndIdx(const Packet& packet, int startIdx) {
    int endBracketIdx = packet.size();
    for (auto i=startIdx+1; i<packet.size(); ++i) {
        SignalChar sc = packet[i];
        if(sc.isPunctuation()){
            endBracketIdx = i;
            break;
        }
    }
    return endBracketIdx;
}

void fixBrackets(Packet& packet, int startIdx) {
    packet.insert(numEndIdx(packet, startIdx), "]");
    packet.insert(startIdx, "[");
}


int getNumber(const Packet& packet, int i) {
    int endIx = numEndIdx(packet, i);
    std::string numStr(packet.begin()+i, packet.begin()+endIx);
    return std::stoi(numStr);
}


CorrectionResult comparePackets(Packet& line1, Packet& line2) {
    CorrectionResult result = CorrectionResult::undetermined;
    int j = 0;
    while (j < std::min(line1.size(), line2.size())) {
        // Mismatch
        SignalChar c1 = line1[j];
        SignalChar c2 = line2[j];

        // If both are numbers, stop if they aren't equal
        if(c1.isNumber() && c2.isNumber()) {
            int val1 = getNumber(line1, j);
            int val2 = getNumber(line2, j);
            if (val1 < val2) {
                result = CorrectionResult::correct;
                break;
            } else if (val1 > val2) {
                result = CorrectionResult::incorrect;
                break;
            } else {
                j++;
            }

        }
        else if(c1.isClosing() && !c2.isClosing()) { // Left list ran out first
            result = CorrectionResult::correct;
            break;
        }
        else if(!c1.isClosing() && c2.isClosing()) { // Right list ran out first
            result = CorrectionResult::incorrect;
            break;
        }
        else if(c1.isOpening() && !c2.isOpening()) { // Update brackets for line 2
            fixBrackets(line2, j);
        }
        else if(!c1.isOpening() && c2.isOpening()) { // Update brackets for line 1
            fixBrackets(line1, j);
        }
        else { // Only increment if we aren't adjusting the size of either list
            j++;
        }
    }

    return result;
}

bool operator< (const Packet& p1, const Packet& p2){
    Packet p1Copy(p1);
    Packet p2Copy(p2);
    CorrectionResult result = comparePackets(p1Copy, p2Copy);
    return (result == CorrectionResult::correct);
}
bool operator> (const Packet& p1, const Packet& p2){
    return !(p1 < p2);
}
bool operator== (const Packet& p1, const Packet& p2) {
    return p1.packet() == p2.packet();
}

auto getPacketIndex(std::vector<Packet> packets, const Packet& element) {
    auto it = std::find(packets.begin(), packets.end(), element);
    return 1 + std::distance(packets.begin(), it);
}
auto runSimulation(bool sample) {
    std::vector<std::string> all_lines = aoc::readLines(13, sample);

    std::vector<int> correctOrderIxs;
    std::vector<Packet> allPackets;
    for(int i=0; i<all_lines.size(); i+=3) {
        int pairNumber = static_cast<int>(i/3) + 1;
        Packet p1(all_lines[i]);
        Packet p2(all_lines[i+1]);

        if (p1 < p2)
            correctOrderIxs.push_back(pairNumber);

        allPackets.push_back(p1);
        allPackets.push_back(p2);
    }

    // Compute the sum for part 1
    int pt1Sum = std::accumulate(correctOrderIxs.begin(), correctOrderIxs.end(), 0);
    std::cout << "Part 1: " << pt1Sum << std::endl;

    // Part 2: Add the two dividers
    Packet dividerOne("[[2]]");
    Packet dividerTwo("[[6]]");
    allPackets.push_back(dividerOne);
    allPackets.push_back(dividerTwo);

    // Sort the list of packets (based on the implemented < operator)
    std::sort(allPackets.begin(), allPackets.end());

    // Extract the requested value
    auto div1Ix = getPacketIndex(allPackets, dividerOne);
    auto div2Ix = getPacketIndex(allPackets, dividerTwo);
    std::cout << "Part 2: " << div1Ix*div2Ix << std::endl;

}


int main() {
    runSimulation(true);
    runSimulation(false);
}