#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <fstream>

// Structure to represent a range of fresh ingredient IDs
struct Range {
    long long start;
    long long end;
    
    Range(long long s, long long e) : start(s), end(e) {}
    
    // Check if a given ID falls within this range
    bool contains(long long id) const {
        return id >= start && id <= end;
    }
};

// Parse input and return the fresh ranges
std::vector<Range> parseRanges(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<Range> freshRanges;
    
    // Parse input to get fresh ingredient ID ranges
    while (std::getline(file, line)) {
        // Empty line separates ranges from available IDs
        if (line.empty()) {
            break; // We only need the ranges
        }
        
        // Parse range in format "start-end"
        size_t dashPos = line.find('-');
        if (dashPos != std::string::npos) {
            long long start = std::stoll(line.substr(0, dashPos));
            long long end = std::stoll(line.substr(dashPos + 1));
            freshRanges.emplace_back(start, end);
        }
    }
    
    return freshRanges;
}

// Parse input and return the available IDs
std::vector<long long> parseAvailableIds(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<long long> availableIds;
    bool parsingRanges = true;
    
    // Skip the ranges section
    while (std::getline(file, line)) {
        if (line.empty()) {
            parsingRanges = false;
            break;
        }
    }
    
    // Parse available IDs
    while (std::getline(file, line)) {
        availableIds.push_back(std::stoll(line));
    }
    
    return availableIds;
}

// Part 1 solution
void solvePart1(const std::string& filename) {
    std::vector<Range> freshRanges = parseRanges(filename);
    std::vector<long long> availableIds = parseAvailableIds(filename);
    
    // Count fresh ingredients
    int freshCount = 0;
    for (long long id : availableIds) {
        bool isFresh = false;
        for (const Range& range : freshRanges) {
            if (range.contains(id)) {
                isFresh = true;
                break;
            }
        }
        if (isFresh) {
            freshCount++;
        }
    }
    
    std::cout << "\nPart 1 Answer: " << freshCount << std::endl;
}

// Part 2 solution - find all unique fresh ingredient IDs
void solvePart2(const std::string& filename) {
    std::vector<Range> freshRanges = parseRanges(filename);
    
    // Debug output
    // std::cout << "Parsed " << freshRanges.size() << " ranges:" << std::endl;
    // for (const auto& range : freshRanges) {
    //     std::cout << "  " << range.start << "-" << range.end << std::endl;
    // }
    
    // Merge overlapping ranges
    std::sort(freshRanges.begin(), freshRanges.end(), 
              [](const Range& a, const Range& b) { return a.start < b.start; });
    
    std::vector<Range> mergedRanges;
    
    if (!freshRanges.empty()) {
        mergedRanges.push_back(freshRanges[0]);
        
        for (size_t i = 1; i < freshRanges.size(); ++i) {
            Range& last = mergedRanges.back();
            const Range& current = freshRanges[i];
            
            // If current range overlaps with last merged range or is adjacent, extend the last range
            if (current.start <= last.end + 1) {
                last.end = std::max(last.end, current.end);
            } else {
                // No overlap, add as a new range
                mergedRanges.push_back(current);
            }
        }
    }
    
    // Debug output
    // std::cout << "Merged into " << mergedRanges.size() << " ranges:" << std::endl;
    // for (const auto& range : mergedRanges) {
    //     std::cout << "  " << range.start << "-" << range.end << std::endl;
    // }
    
    // Count total unique IDs in all merged ranges
    long long totalFreshIds = 0;
    for (const Range& range : mergedRanges) {
        totalFreshIds += (range.end - range.start + 1);
    }
    
    std::cout << "Part 2 Answer: " << totalFreshIds << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename = "example.in";
    
    if (argc > 1) {
        filename = argv[1];
    }
    
    // Solve part 1
    solvePart1(filename);
    
    // Solve part 2
    solvePart2(filename);
    
    return 0;
}
