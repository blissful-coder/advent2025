#!/bin/bash

# Check if day number is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <day_number>"
    exit 1
fi

DAY=$1
DAY_DIR="day$DAY"
CPP_FILE="$DAY_DIR/day$DAY.cpp"
INPUT_FILE="$DAY_DIR/day$DAY.in"
EXAMPLE_FILE="$DAY_DIR/example.in"

# Create directory if it doesn't exist
mkdir -p "$DAY_DIR"

# Create C++ template file if it doesn't exist
if [ ! -f "$CPP_FILE" ]; then
    cat > "$CPP_FILE" << 'EOF'
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

// Part 1 solution
void solvePart1() {
    std::string line;
    int result = 0;
    
    while (std::getline(std::cin, line)) {
        // Process each line of input
        
    }
    
    std::cout << "\nPart 1 Answer: " << result << std::endl;
}

// Part 2 solution
void solvePart2() {
    std::string line;
    int result = 0;
    
    while (std::getline(std::cin, line)) {
        // Process each line of input
        
    }
    
    std::cout << "Part 2 Answer: " << result << std::endl;
}

int main() {
    // Solve both parts
    solvePart1();
    
    // Reset input stream to beginning
    std::cin.clear();
    std::cin.seekg(0);
    
    solvePart2();
    
    return 0;
}
EOF
    echo "Created C++ template file: $CPP_FILE"
else
    echo "C++ file already exists: $CPP_FILE"
fi

# Create empty input files if they don't exist
if [ ! -f "$INPUT_FILE" ]; then
    touch "$INPUT_FILE"
    echo "Created empty input file: $INPUT_FILE"
else
    echo "Input file already exists: $INPUT_FILE"
fi

if [ ! -f "$EXAMPLE_FILE" ]; then
    touch "$EXAMPLE_FILE"
    echo "Created empty example file: $EXAMPLE_FILE"
else
    echo "Example file already exists: $EXAMPLE_FILE"
fi

echo "Day $DAY initialized successfully!"
echo ""
echo "To compile and run with example input:"
echo "g++ -std=c++17 $CPP_FILE -o $DAY_DIR/day$DAY && $DAY_DIR/day$DAY < $EXAMPLE_FILE"
echo ""
echo "To compile and run with actual input:"
echo "g++ -std=c++17 $CPP_FILE -o $DAY_DIR/day$DAY && $DAY_DIR/day$DAY < $INPUT_FILE"
