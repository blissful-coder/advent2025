#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

// Part 1 solution
int solvePart1(const std::vector<std::string>& lines) {
    int rows = lines.size();
    int cols = lines[0].size();
    int result = 0;
    
    // Define the 8 possible directions (including diagonals)
    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Check if current position is a paper roll
            if (lines[i][j] == '@') {
                // Count adjacent paper rolls
                int adjacentRolls = 0;
                for (int dir = 0; dir < 8; ++dir) {
                    int ni = i + dx[dir];
                    int nj = j + dy[dir];
                    
                    // Check if the adjacent position is within bounds and has a paper roll
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && lines[ni][nj] == '@') {
                        adjacentRolls++;
                    }
                }
                
                // If there are fewer than 4 adjacent paper rolls, this roll can be accessed
                if (adjacentRolls < 4) {
                    result++;
                }
            }
        }
    }
    
    return result;
}

// Part 2 solution
long long solvePart2(const std::vector<std::string>& lines) {
    int rows = lines.size();
    int cols = lines[0].size();
    long long result = 0;
    
    // Create a mutable copy of the grid
    std::vector<std::string> grid = lines;
    
    // Define the 8 possible directions (including diagonals)
    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    // Continue removing paper rolls until no more can be removed
    bool removedAny = true;
    while (removedAny) {
        removedAny = false;
        
        // Find all accessible paper rolls in the current state
        std::vector<std::pair<int, int>> accessibleRolls;
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // Check if current position is a paper roll
                if (grid[i][j] == '@') {
                    // Count adjacent paper rolls
                    int adjacentRolls = 0;
                    for (int dir = 0; dir < 8; ++dir) {
                        int ni = i + dx[dir];
                        int nj = j + dy[dir];
                        
                        // Check if the adjacent position is within bounds and has a paper roll
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && grid[ni][nj] == '@') {
                            adjacentRolls++;
                        }
                    }
                    
                    // If there are fewer than 4 adjacent paper rolls, this roll can be accessed
                    if (adjacentRolls < 4) {
                        accessibleRolls.push_back({i, j});
                    }
                }
            }
        }
        
        // Remove all accessible paper rolls
        for (const auto& pos : accessibleRolls) {
            grid[pos.first][pos.second] = '.';
            result++;
            removedAny = true;
        }
    }
    
    return result;
}

int main() {
    // Read input into memory
    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(std::cin, line)) {
        lines.push_back(line);
    }
    
    // Solve both parts
    int part1Result = solvePart1(lines);
    long long part2Result = solvePart2(lines);
    
    std::cout << "\nPart 1 Answer: " << part1Result << std::endl;
    std::cout << "Part 2 Answer: " << part2Result << std::endl;
    
    return 0;
}
