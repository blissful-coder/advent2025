#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

bool DEBUG = false;

void debug(const std::string& msg) {
    if (DEBUG) {
        std::cout << msg << std::endl;
    }
}

struct Box {
    std::vector<std::string> pattern;
};

struct Bin {
    std::pair<int, int> size; // width x height
    std::vector<int> counts;  // count of each box type
};

std::pair<std::vector<Box>, std::vector<Bin>> parse_input() {
    std::vector<Box> boxes;
    std::vector<Bin> bins;
    std::string line;
    
    Box current_box;
    bool in_box = false;
    
    while (std::getline(std::cin, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Check if this is a box header (ends with ':')
        if (line.back() == ':') {
            if (in_box) {
                boxes.push_back(current_box);
                current_box.pattern.clear();
            }
            in_box = true;
        }
        // Check if this is a box pattern line (starts with '.' or '#')
        else if (in_box && (line[0] == '.' || line[0] == '#')) {
            current_box.pattern.push_back(line);
        }
        // Otherwise, it's a bin definition
        else if (line.find(": ") != std::string::npos) {
            // If we were parsing a box, add it to the boxes vector
            if (in_box) {
                boxes.push_back(current_box);
                current_box.pattern.clear();
                in_box = false;
            }
            
            // Parse bin size and counts
            size_t colon_pos = line.find(": ");
            std::string size_str = line.substr(0, colon_pos);
            std::string counts_str = line.substr(colon_pos + 2);
            
            // Parse size (WxH)
            size_t x_pos = size_str.find('x');
            int width = std::stoi(size_str.substr(0, x_pos));
            int height = std::stoi(size_str.substr(x_pos + 1));
            
            // Parse counts
            std::vector<int> counts;
            std::istringstream iss(counts_str);
            int count;
            while (iss >> count) {
                counts.push_back(count);
            }
            
            bins.push_back({std::make_pair(width, height), counts});
        }
    }
    
    // Add the last box if we were parsing one
    if (in_box) {
        boxes.push_back(current_box);
    }
    
    return {boxes, bins};
}

int part1(const std::vector<Box>& boxes, const std::vector<Bin>& bins) {
    // Calculate the area of each box (count of '#' characters)
    std::unordered_map<int, int> box_areas;
    for (size_t i = 0; i < boxes.size(); ++i) {
        int area = 0;
        for (const auto& row : boxes[i].pattern) {
            for (char c : row) {
                if (c == '#') {
                    area++;
                }
            }
        }
        box_areas[i] = area;
    }
    
    // Add 1 to the area of box 1 (hole that can't be reached)
    box_areas[1] += 1;
    
    if (DEBUG) {
        std::cout << "Box areas:" << std::endl;
        for (const auto& [idx, area] : box_areas) {
            std::cout << idx << ": " << area << std::endl;
        }
    }
    
    // Count bins where the needed area is less than the available area
    int total = 0;
    for (const auto& bin : bins) {
        int area = bin.size.first * bin.size.second;
        int needed = 0;
        
        // Calculate total area needed based on box areas and counts
        for (size_t i = 0; i < bin.counts.size(); ++i) {
            needed += box_areas[i] * bin.counts[i];
        }
        
        if (needed < area) {
            total++;
        } else if (DEBUG) {
            std::cout << "Size: " << bin.size.first << "x" << bin.size.second 
                      << ", Area: " << area << ", Needed: " << needed << std::endl;
        }
    }
    
    return total;
}

int main(int argc, char* argv[]) {
    // Check for debug flag
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-v") {
            DEBUG = true;
        }
    }
    
    auto [boxes, bins] = parse_input();
    
    // Check for part1 flag
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "1") {
            std::cout << part1(boxes, bins) << std::endl;
            return 0;
        }
    }
    
    // Default to part1 if no part specified
    std::cout << part1(boxes, bins) << std::endl;
    
    return 0;
}
