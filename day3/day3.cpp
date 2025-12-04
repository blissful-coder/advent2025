#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

// Part 1 solution
int solvePart1(const std::vector<std::string>& lines) {
    int result = 0;
    
    for (const std::string& line : lines) {
        // Process each line of input
        int maxJoltage = 0;
        
        // Check all possible pairs of positions
        for (size_t i = 0; i < line.length(); ++i) {
            for (size_t j = i + 1; j < line.length(); ++j) {
                // Calculate the joltage from this pair
                int joltage = (line[i] - '0') * 10 + (line[j] - '0');
                
                // Update max joltage if this is higher
                maxJoltage = std::max(maxJoltage, joltage);
            }
        }
        
        // Add this bank's max joltage to the result
        result += maxJoltage;
    }
    
    return result;
}

// Part 2 solution
long long solvePart2(const std::vector<std::string>& lines) {
    long long result = 0;
    
    for (const std::string& line : lines) {
        // Process each line of input
        
        // We need to select exactly 12 batteries (digits) from each bank
        // to form the largest possible number
        
        // If the line has exactly 12 digits, use all of them
        if (line.length() == 12) {
            long long joltage = 0;
            for (char c : line) {
                joltage = joltage * 10 + (c - '0');
            }
            result += joltage;
            continue;
        }
        
        // If the line has more than 12 digits, we need to remove (n - 12) digits
        // to get exactly 12 digits
        int digitsToRemove = line.length() - 12;
        
        // Use a stack to keep track of the digits we want to keep
        std::vector<char> stack;
        
        for (char c : line) {
            // While we still have digits to remove and the current digit is larger than the last digit in the stack,
            // we remove the last digit from the stack
            while (digitsToRemove > 0 && !stack.empty() && stack.back() < c) {
                stack.pop_back();
                digitsToRemove--;
            }
            
            // Add the current digit to the stack
            stack.push_back(c);
        }
        
        // If we still have digits to remove, remove them from the end
        while (digitsToRemove > 0) {
            stack.pop_back();
            digitsToRemove--;
        }
        
        // Calculate the joltage from the selected digits
        long long joltage = 0;
        for (char c : stack) {
            joltage = joltage * 10 + (c - '0');
        }
        
        // Add this bank's joltage to the result
        result += joltage;
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
