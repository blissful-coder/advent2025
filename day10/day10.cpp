#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <regex>
#include <limits>
#include <functional>

// Parse the indicator light pattern from a line
std::vector<bool> parseIndicatorPattern(const std::string& line) {
    std::vector<bool> pattern;
    size_t start = line.find('[') + 1;
    size_t end = line.find(']');
    
    for (size_t i = start; i < end; i++) {
        pattern.push_back(line[i] == '#');
    }
    
    return pattern;
}

// Parse the button wiring schematics from a line
std::vector<std::vector<int>> parseButtonWirings(const std::string& line) {
    std::vector<std::vector<int>> buttons;
    std::regex buttonRegex("\\(([0-9,]+)\\)");
    
    auto buttons_begin = std::sregex_iterator(line.begin(), line.end(), buttonRegex);
    auto buttons_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = buttons_begin; i != buttons_end; ++i) {
        std::smatch match = *i;
        std::string buttonStr = match[1].str();
        std::vector<int> button;
        
        std::stringstream ss(buttonStr);
        std::string token;
        while (std::getline(ss, token, ',')) {
            button.push_back(std::stoi(token));
        }
        
        buttons.push_back(button);
    }
    
    return buttons;
}

// Find the minimum number of button presses to achieve the target pattern
int findMinButtonPresses(const std::vector<bool>& targetPattern, const std::vector<std::vector<int>>& buttons) {
    int numLights = targetPattern.size();
    int numButtons = buttons.size();
    
    // Create a matrix where each row represents a button's effect on the lights
    std::vector<std::vector<bool>> matrix(numButtons, std::vector<bool>(numLights, false));
    
    // Fill the matrix based on button wirings
    for (int i = 0; i < numButtons; i++) {
        for (int light : buttons[i]) {
            if (light < numLights) {
                matrix[i][light] = true;
            }
        }
    }
    
    // Try all possible combinations of button presses (2^numButtons)
    int minPresses = numButtons + 1; // Initialize to a value larger than possible
    
    for (int mask = 0; mask < (1 << numButtons); mask++) {
        std::vector<bool> currentState(numLights, false);
        int pressCount = 0;
        
        // Apply button presses based on the current mask
        for (int i = 0; i < numButtons; i++) {
            if (mask & (1 << i)) {
                pressCount++;
                // Toggle lights affected by this button
                for (int j = 0; j < numLights; j++) {
                    if (matrix[i][j]) {
                        currentState[j] = !currentState[j];
                    }
                }
            }
        }
        
        // Check if we've reached the target pattern
        bool matches = true;
        for (int i = 0; i < numLights; i++) {
            if (currentState[i] != targetPattern[i]) {
                matches = false;
                break;
            }
        }
        
        if (matches && pressCount < minPresses) {
            minPresses = pressCount;
        }
    }
    
    return minPresses;
}

// Part 1 solution
void solvePart1() {
    std::string line;
    int result = 0;
    
    while (std::getline(std::cin, line)) {
        // Parse the indicator light pattern
        std::vector<bool> targetPattern = parseIndicatorPattern(line);
        
        // Parse the button wiring schematics
        std::vector<std::vector<int>> buttons = parseButtonWirings(line);
        
        // Find the minimum number of button presses
        int minPresses = findMinButtonPresses(targetPattern, buttons);
        
        // Add to the total
        result += minPresses;
    }
    
    std::cout << "\nPart 1 Answer: " << result << std::endl;
}

// Fraction class to handle rational numbers
class Fraction {
public:
    long long numerator;
    long long denominator;
    
    Fraction(long long num = 0, long long den = 1) : numerator(num), denominator(den) {
        if (denominator == 0) {
            throw std::runtime_error("Denominator cannot be zero");
        }
        simplify();
    }
    
    void simplify() {
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        
        long long gcd_val = gcd(std::abs(numerator), denominator);
        if (gcd_val > 1) {
            numerator /= gcd_val;
            denominator /= gcd_val;
        }
    }
    
    long long gcd(long long a, long long b) {
        while (b) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    
    Fraction operator+(const Fraction& other) const {
        long long new_num = numerator * other.denominator + other.numerator * denominator;
        long long new_den = denominator * other.denominator;
        return Fraction(new_num, new_den);
    }
    
    Fraction operator-(const Fraction& other) const {
        long long new_num = numerator * other.denominator - other.numerator * denominator;
        long long new_den = denominator * other.denominator;
        return Fraction(new_num, new_den);
    }
    
    Fraction operator*(const Fraction& other) const {
        long long new_num = numerator * other.numerator;
        long long new_den = denominator * other.denominator;
        return Fraction(new_num, new_den);
    }
    
    Fraction operator/(const Fraction& other) const {
        if (other.numerator == 0) {
            throw std::runtime_error("Division by zero");
        }
        long long new_num = numerator * other.denominator;
        long long new_den = denominator * other.numerator;
        return Fraction(new_num, new_den);
    }
    
    bool operator==(const Fraction& other) const {
        return numerator * other.denominator == other.numerator * denominator;
    }
    
    bool operator==(int value) const {
        return numerator == value * denominator;
    }
    
    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }
    
    bool operator!=(int value) const {
        return !(*this == value);
    }
    
    bool operator<(const Fraction& other) const {
        return numerator * other.denominator < other.numerator * denominator;
    }
    
    bool operator>(const Fraction& other) const {
        return numerator * other.denominator > other.numerator * denominator;
    }
    
    // Only needed operators are included for simplicity
};

// Solve a linear programming problem for Part 2 using Gaussian elimination
int solveLinearProgramming(const std::vector<int>& targetJoltages, const std::vector<std::vector<int>>& buttons) {
    int numCounters = targetJoltages.size();
    int numButtons = buttons.size();
    int max_exp = *std::max_element(targetJoltages.begin(), targetJoltages.end());
    
    // Create a matrix for Gaussian elimination
    // Each row represents a counter, each column represents a button
    std::vector<std::vector<Fraction>> matrix(numCounters, std::vector<Fraction>(numButtons + 1, Fraction(0, 1)));
    
    // Fill the matrix with the target joltages in the last column
    for (int i = 0; i < numCounters; i++) {
        matrix[i][numButtons] = Fraction(targetJoltages[i], 1);
    }
    
    // Fill the matrix with button effects
    for (int i = 0; i < numButtons; i++) {
        for (int counter : buttons[i]) {
            if (counter < numCounters) {
                matrix[counter][i] = Fraction(1, 1);
            }
        }
    }
    
    // Perform Gaussian elimination
    int row_idx = 0;
    std::vector<int> free_vars;
    
    for (int i = 0; i < numButtons; i++) {
        // Find the next row with a non-zero coefficient for the current column
        int next_row = -1;
        for (int j = row_idx; j < numCounters; j++) {
            if (matrix[j][i].numerator != 0) {
                next_row = j;
                break;
            }
        }
        
        // If no such row exists, this is a free variable
        if (next_row == -1) {
            free_vars.push_back(i);
            continue;
        }
        
        // Swap rows to bring the pivot to the current row
        if (next_row != row_idx) {
            std::swap(matrix[row_idx], matrix[next_row]);
        }
        
        // Eliminate the variable from all other rows
        for (int k = 0; k < numCounters; k++) {
            if (k == row_idx) continue;
            
            Fraction factor = matrix[k][i] / matrix[row_idx][i];
            for (int j = 0; j <= numButtons; j++) {
                matrix[k][j] = matrix[k][j] - matrix[row_idx][j] * factor;
            }
        }
        
        row_idx++;
    }
    
    // Convert fractions to integers where possible
    for (auto& row : matrix) {
        for (auto& cell : row) {
            if (cell.denominator == 1) {
                // It's already an integer
                continue;
            }
            // Check if it's an integer
            if (cell.numerator % cell.denominator == 0) {
                cell = Fraction(cell.numerator / cell.denominator, 1);
            }
        }
    }
    
    // Try all possible combinations of free variables
    int realans = -1;
    
    // Generate all combinations of values for free variables
    std::function<void(int, std::vector<int>&, int)> tryFreeVars = 
        [&](int idx, std::vector<int>& values, int current_sum) {
            // If we've already found a better answer, prune this branch
            if (realans != -1 && current_sum >= realans) {
                return;
            }
            
            // If we've assigned all free variables, check if this is a valid solution
            if (idx == free_vars.size()) {
                bool good = true;
                int ans = current_sum;
                
                for (const auto& row : matrix) {
                    // Skip rows that are all zeros
                    bool all_zeros = true;
                    for (const auto& cell : row) {
                        if (cell.numerator != 0) {
                            all_zeros = false;
                            break;
                        }
                    }
                    if (all_zeros) continue;
                    
                    // Calculate the value for this row
                    Fraction subans = row[numButtons];
                    for (int i = 0; i < free_vars.size(); i++) {
                        subans = subans - row[free_vars[i]] * Fraction(values[i], 1);
                    }
                    
                    // Find the first non-zero coefficient
                    Fraction first_nonzero(0, 1);
                    for (int j = 0; j < numButtons; j++) {
                        if (std::find(free_vars.begin(), free_vars.end(), j) == free_vars.end() && 
                            row[j].numerator != 0) {
                            first_nonzero = row[j];
                            break;
                        }
                    }
                    
                    // Check if the solution is valid
                    if (first_nonzero.numerator != 0) {
                        Fraction result = subans / first_nonzero;
                        
                        // Check if result is a non-negative integer
                        if (result.denominator != 1 || result.numerator < 0) {
                            good = false;
                            break;
                        }
                        
                        ans += result.numerator;
                        
                        // If we've already found a better answer, prune this branch
                        if (realans != -1 && ans >= realans) {
                            good = false;
                            break;
                        }
                    }
                }
                
                if (good) {
                    if (realans == -1 || ans < realans) {
                        realans = ans;
                    }
                }
                return;
            }
            
            // Try all possible values for the current free variable
            for (int val = 0; val <= max_exp; val++) {
                values[idx] = val;
                tryFreeVars(idx + 1, values, current_sum + val);
                
                // If we've already found a better answer, prune this branch
                if (realans != -1 && current_sum + val >= realans) {
                    break;
                }
            }
        };
    
    std::vector<int> values(free_vars.size(), 0);
    tryFreeVars(0, values, 0);
    
    return realans;
}

// Part 2 solution
void solvePart2() {
    std::string line;
    int result = 0;
    
    while (std::getline(std::cin, line)) {
        // Parse the button wiring schematics
        std::vector<std::vector<int>> buttons = parseButtonWirings(line);
        
        // Parse the joltage requirements - extract the part between { and }
        std::vector<int> targetJoltages;
        size_t start = line.find('{');
        size_t end = line.find('}');
        
        if (start != std::string::npos && end != std::string::npos) {
            std::string joltageStr = line.substr(start + 1, end - start - 1);
            std::stringstream ss(joltageStr);
            std::string token;
            
            while (std::getline(ss, token, ',')) {
                targetJoltages.push_back(std::stoi(token));
            }
        }
        
        // Solve the linear programming problem
        int minPresses = solveLinearProgramming(targetJoltages, buttons);
        
        // Add to the total
        result += minPresses;
    }
    
    std::cout << "Part 2 Answer: " << result << std::endl;
}

int main() {
    // Read the entire input into a string
    std::string input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }
    
    // Create a stringstream for Part 1
    std::istringstream iss1(input);
    std::cin.rdbuf(iss1.rdbuf());
    
    // Solve Part 1
    solvePart1();
    
    // Create a new stringstream for Part 2
    std::istringstream iss2(input);
    std::cin.rdbuf(iss2.rdbuf());
    
    // Solve Part 2
    solvePart2();
    
    return 0;
}
