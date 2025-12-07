#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
#include <sstream>

// Function to solve a single math problem
long long solveProblem(const std::vector<long long>& numbers, char operation) {
    long long result = 0;
    
    // Apply the operation to all numbers
    if (operation == '+') {
        result = 0;
        for (const auto& num : numbers) {
            result += num;
        }
    } else if (operation == '*') {
        result = 1;
        for (const auto& num : numbers) {
            result *= num;
        }
    }
    
    return result;
}

// Function to check if a column is empty (all spaces)
bool isEmptyColumn(const std::vector<std::string>& lines, size_t col) {
    for (const auto& line : lines) {
        if (col < line.size() && line[col] != ' ') {
            return false;
        }
    }
    return true;
}

// Function to find the boundaries of each problem
std::vector<std::pair<int, int>> findProblemBoundaries(const std::vector<std::string>& lines) {
    std::vector<std::pair<int, int>> boundaries;
    
    // Find the maximum line length
    size_t maxLength = 0;
    for (const auto& line : lines) {
        maxLength = std::max(maxLength, line.size());
    }
    
    // Find columns with operations
    std::vector<int> operationCols;
    const std::string& operationsLine = lines.back();
    for (size_t i = 0; i < operationsLine.size(); ++i) {
        if (operationsLine[i] == '+' || operationsLine[i] == '*') {
            operationCols.push_back(i);
        }
    }
    
    // Group operation columns into problems
    for (size_t i = 0; i < operationCols.size(); ++i) {
        int start = operationCols[i];
        
        // Find the left boundary (first column of the problem)
        int left = start;
        while (left > 0) {
            if (isEmptyColumn(lines, left - 1)) {
                break;
            }
            left--;
        }
        
        // Find the right boundary (last column of the problem)
        int right = start;
        while (right < maxLength - 1) {
            if (isEmptyColumn(lines, right + 1)) {
                break;
            }
            right++;
        }
        
        boundaries.push_back({left, right});
    }
    
    return boundaries;
}

// Function to extract a number from a problem column range (Part 1)
long long extractNumberPart1(const std::vector<std::string>& lines, size_t lineIndex, int left, int right) {
    if (lineIndex >= lines.size()) {
        return 0;
    }
    
    const std::string& line = lines[lineIndex];
    std::string numStr;
    
    for (int i = left; i <= right; ++i) {
        if (i < line.size()) {
            numStr += line[i];
        }
    }
    
    // Remove any leading/trailing spaces
    numStr.erase(0, numStr.find_first_not_of(" \t\n\r\f\v"));
    numStr.erase(numStr.find_last_not_of(" \t\n\r\f\v") + 1);
    
    if (numStr.empty()) {
        return 0;
    }
    
    return std::stoll(numStr);
}

// Function to extract a number from a problem column (Part 2)
// In Part 2, numbers are read vertically with most significant digit at the top
long long extractNumberPart2(const std::vector<std::string>& lines, int col, int numLines) {
    std::string numStr;
    
    // Read digits from top to bottom
    for (int i = 0; i < numLines; ++i) {
        if (col < lines[i].size() && lines[i][col] != ' ') {
            numStr += lines[i][col];
        }
    }
    
    if (numStr.empty()) {
        return 0;
    }
    
    return std::stoll(numStr);
}

// Part 1 solution
void solvePart1(const std::vector<std::string>& lines = {}) {
    std::vector<std::string> inputLines = lines;
    long long grandTotal = 0;
    
    // If no lines were provided, read from stdin
    if (inputLines.empty()) {
        std::string line;
        while (std::getline(std::cin, line)) {
            inputLines.push_back(line);
        }
    }
    
    // Make sure we have at least one line for the operations
    if (inputLines.empty()) {
        std::cout << "\nPart 1 Answer: 0 (No input)" << std::endl;
        return;
    }
    
    // Find the boundaries of each problem
    std::vector<std::pair<int, int>> problemBoundaries = findProblemBoundaries(inputLines);
    
    // Process each problem
    for (const auto& [left, right] : problemBoundaries) {
        // Find the operation for this problem
        char operation = ' ';
        const std::string& operationsLine = inputLines.back();
        for (int i = left; i <= right; ++i) {
            if (i < operationsLine.size() && (operationsLine[i] == '+' || operationsLine[i] == '*')) {
                operation = operationsLine[i];
                break;
            }
        }
        
        if (operation == ' ') {
            continue; // Skip if no operation found
        }
        
        // Extract numbers for this problem
        std::vector<long long> numbers;
        for (size_t i = 0; i < inputLines.size() - 1; ++i) {
            long long num = extractNumberPart1(inputLines, i, left, right);
            if (num != 0) {
                numbers.push_back(num);
            }
        }
        
        // Solve the problem and add to grand total
        long long problemResult = solveProblem(numbers, operation);
        grandTotal += problemResult;
        
        // Debug output
        std::cout << "Problem from column " << left << " to " << right << " with operation " << operation << ":" << std::endl;
        for (const auto& num : numbers) {
            std::cout << num << std::endl;
        }
        std::cout << "Result: " << problemResult << std::endl << std::endl;
    }
    
    std::cout << "\nPart 1 Answer: " << grandTotal << std::endl;
}

// Part 2 solution
void solvePart2(const std::vector<std::string>& lines = {}) {
    std::vector<std::string> inputLines = lines;
    long long grandTotal = 0;
    
    // If no lines were provided, read from stdin
    if (inputLines.empty()) {
        std::string line;
        while (std::getline(std::cin, line)) {
            inputLines.push_back(line);
        }
    }
    
    // Make sure we have at least one line for the operations
    if (inputLines.empty()) {
        std::cout << "\nPart 2 Answer: 0 (No input)" << std::endl;
        return;
    }
    
    // Find the boundaries of each problem
    std::vector<std::pair<int, int>> problemBoundaries = findProblemBoundaries(inputLines);
    
    // Process each problem
    for (const auto& [left, right] : problemBoundaries) {
        // Find the operation for this problem
        char operation = ' ';
        const std::string& operationsLine = inputLines.back();
        for (int i = left; i <= right; ++i) {
            if (i < operationsLine.size() && (operationsLine[i] == '+' || operationsLine[i] == '*')) {
                operation = operationsLine[i];
                break;
            }
        }
        
        if (operation == ' ') {
            continue; // Skip if no operation found
        }
        
        // Extract numbers for this problem (reading right-to-left, column by column)
        std::vector<long long> numbers;
        int numLines = inputLines.size() - 1; // Exclude the operations line
        
        for (int col = right; col >= left; --col) {
            long long num = extractNumberPart2(inputLines, col, numLines);
            if (num != 0) {
                numbers.push_back(num);
            }
        }
        
        // Solve the problem and add to grand total
        long long problemResult = solveProblem(numbers, operation);
        grandTotal += problemResult;
        
        // Debug output
        std::cout << "Problem from column " << left << " to " << right << " with operation " << operation << ":" << std::endl;
        for (const auto& num : numbers) {
            std::cout << num << std::endl;
        }
        std::cout << "Result: " << problemResult << std::endl << std::endl;
    }
    
    std::cout << "\nPart 2 Answer: " << grandTotal << std::endl;
}


int main(int argc, char* argv[]) {
    // Check if a file path is provided
    if (argc > 1) {
        std::cout << "Opening file: " << argv[1] << std::endl;
        
        // Open the file
        std::ifstream inputFile(argv[1]);
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        
        // Read the entire file content
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(inputFile, line)) {
            std::cout << "Read line: " << line << std::endl;
            lines.push_back(line);
        }
        
        // Check if we should solve part 1 or part 2
        if (argc > 2 && std::string(argv[2]) == "part2") {
            // Solve part 2 with the lines we've read
            solvePart2(lines);
        } else {
            // Solve part 1 with the lines we've read
            solvePart1(lines);
        }
    } else {
        // No file provided, read from stdin
        solvePart1();
    }
    
    return 0;
}
