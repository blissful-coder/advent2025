#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
#include <queue>
#include <utility>
#include <tuple>
#include <functional>

// Structure to represent a beam
struct Beam {
    int row;
    int col;
    
    Beam(int r, int c) : row(r), col(c) {}
};

// Function to simulate beam propagation and count splits (Part 1)
int simulateBeam(const std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    
    std::cout << "Grid size: " << rows << " x " << cols << std::endl;
    
    // Find the starting position 'S'
    int startRow = -1;
    int startCol = -1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'S') {
                startRow = i;
                startCol = j;
                break;
            }
        }
        if (startRow != -1) break;
    }
    
    std::cout << "Starting position: (" << startRow << ", " << startCol << ")" << std::endl;
    
    // Initialize with the beam starting from 'S' and moving downward
    std::queue<Beam> beams;
    beams.push(Beam(startRow + 1, startCol)); // Start one row below 'S'
    
    // Keep track of visited splitters to avoid counting them multiple times
    std::set<std::pair<int, int>> visitedSplitters;
    
    // Count of beam splits
    int splitCount = 0;
    
    // Process beams until no more beams are left
    while (!beams.empty()) {
        Beam current = beams.front();
        beams.pop();
        
        // Check if the beam is out of bounds
        if (current.row < 0 || current.row >= rows || current.col < 0 || current.col >= cols) {
            continue;
        }
        
        // Check if the beam encounters a splitter
        if (grid[current.row][current.col] == '^') {
            // Check if this splitter has been visited before
            std::pair<int, int> splitterPos = {current.row, current.col};
            if (visitedSplitters.find(splitterPos) == visitedSplitters.end()) {
                // Mark the splitter as visited
                visitedSplitters.insert(splitterPos);
                
                // Count this as a split
                splitCount++;
                
                std::cout << "Beam split at position (" << current.row << ", " << current.col << ")" << std::endl;
                
                // Create two new beams from the left and right of the splitter
                beams.push(Beam(current.row + 1, current.col - 1)); // Left beam
                beams.push(Beam(current.row + 1, current.col + 1)); // Right beam
            }
        } else {
            // Continue the beam downward
            beams.push(Beam(current.row + 1, current.col));
        }
    }
    
    return splitCount;
}

// Function to count the number of unique timelines (Part 2)
long long countTimelines(const std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    
    // Find the starting position 'S'
    int startRow = -1;
    int startCol = -1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'S') {
                startRow = i;
                startCol = j;
                break;
            }
        }
        if (startRow != -1) break;
    }
    
    std::cout << "Starting position: (" << startRow << ", " << startCol << ")" << std::endl;
    
    // Create a 2D array to store the number of paths to each position
    std::vector<std::vector<long long>> pathCount(rows + 1, std::vector<long long>(cols + 2, 0));
    
    // Initialize the starting position
    pathCount[startRow + 1][startCol + 1] = 1;
    
    // Process each row from top to bottom
    for (int r = startRow + 1; r < rows; ++r) {
        for (int c = 1; c <= cols; ++c) {
            // Skip if no paths reach this position
            if (pathCount[r][c] == 0) continue;
            
            // If this is a splitter, paths go to both left and right in the next row
            if (grid[r-1][c-1] == '^') {
                // Left path
                pathCount[r + 1][c - 1] += pathCount[r][c];
                
                // Right path
                pathCount[r + 1][c + 1] += pathCount[r][c];
            } else {
                // Continue downward
                pathCount[r + 1][c] += pathCount[r][c];
            }
        }
    }
    
    // Count paths that reach the bottom row or go out of bounds
    long long totalPaths = 0;
    
    // Paths that reach the bottom row
    for (int c = 1; c <= cols; ++c) {
        totalPaths += pathCount[rows][c];
    }
    
    // Paths that go out of bounds from the sides
    for (int r = startRow + 1; r <= rows; ++r) {
        totalPaths += pathCount[r][0];     // Left edge
        totalPaths += pathCount[r][cols+1]; // Right edge
    }
    
    std::cout << "Total unique paths (timelines): " << totalPaths << std::endl;
    
    return totalPaths;
}

// Part 1 solution
void solvePart1(const std::string& filename = "") {
    std::vector<std::string> grid;
    
    if (!filename.empty()) {
        // Read directly from the file
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " in solvePart1" << std::endl;
            return;
        }
        
        std::string line;
        while (std::getline(inputFile, line)) {
            grid.push_back(line);
        }
    } else {
        // Read from stdin
        std::string line;
        while (std::getline(std::cin, line)) {
            grid.push_back(line);
        }
    }
    
    std::cout << "Read " << grid.size() << " lines from input" << std::endl;
    
    // Print the first few lines of the grid for debugging
    for (int i = 0; i < std::min(5, (int)grid.size()); ++i) {
        std::cout << "Line " << i << ": " << grid[i] << std::endl;
    }
    
    // Simulate beam propagation and count splits
    int result = simulateBeam(grid);
    
    std::cout << "\nPart 1 Answer: " << result << std::endl;
}

// Part 2 solution
void solvePart2(const std::string& filename = "") {
    std::vector<std::string> grid;
    
    if (!filename.empty()) {
        // Read directly from the file
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " in solvePart2" << std::endl;
            return;
        }
        
        std::string line;
        while (std::getline(inputFile, line)) {
            grid.push_back(line);
        }
    } else {
        // Read from stdin
        std::string line;
        while (std::getline(std::cin, line)) {
            grid.push_back(line);
        }
    }
    
    std::cout << "Read " << grid.size() << " lines from input" << std::endl;
    
    // Count the number of unique timelines
    long long result = countTimelines(grid);
    
    std::cout << "\nPart 2 Answer: " << result << std::endl;
}

int main(int argc, char* argv[]) {
    // Check if a file path is provided
    if (argc > 1) {
        std::cout << "Using file: " << argv[1] << std::endl;
        
        // Check if we should solve part 1 or part 2
        if (argc > 2 && std::string(argv[2]) == "part2") {
            // Solve part 2 with the provided file
            solvePart2(argv[1]);
        } else {
            // Solve part 1 with the provided file
            solvePart1(argv[1]);
        }
    } else {
        std::cout << "No input file provided, reading from stdin" << std::endl;
        
        // Solve part 1 with stdin
        solvePart1();
    }
    
    return 0;
}
