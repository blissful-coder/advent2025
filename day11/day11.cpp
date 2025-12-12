#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <fstream>

// Graph representation using adjacency list
using Graph = std::map<std::string, std::vector<std::string>>;

// DFS function to find all paths from source to destination
void dfs(const Graph& graph, const std::string& current, const std::string& destination, 
         std::set<std::string>& visited, int& pathCount) {
    
    // If we reached the destination, increment path count
    if (current == destination) {
        pathCount++;
        return;
    }
    
    // Mark current node as visited
    visited.insert(current);
    
    // Explore all adjacent nodes
    if (graph.find(current) != graph.end()) {
        for (const std::string& neighbor : graph.at(current)) {
            // Only visit if not already visited
            if (visited.find(neighbor) == visited.end()) {
                dfs(graph, neighbor, destination, visited, pathCount);
            }
        }
    }
    
    // Backtrack: remove current node from visited set
    visited.erase(current);
}

// Key for memoization: node, visited dac, visited fft
using MemoKey = std::tuple<std::string, bool, bool>;

// Memoized DFS function for part 2 - find paths that visit both dac and fft
long long memoizedDFS(const Graph& graph, const std::string& current, const std::string& destination,
                    std::set<std::string>& visited, bool visitedDac, bool visitedFft,
                    std::map<MemoKey, long long>& memo) {
    
    // Check if we've already computed this state
    MemoKey key = std::make_tuple(current, visitedDac, visitedFft);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    
    // Update visited flags
    bool newVisitedDac = visitedDac || (current == "dac");
    bool newVisitedFft = visitedFft || (current == "fft");
    
    // If we reached the destination, return 1 if we've visited both required nodes, 0 otherwise
    if (current == destination) {
        return (newVisitedDac && newVisitedFft) ? 1 : 0;
    }
    
    // Mark current node as visited
    visited.insert(current);
    
    // Count paths from this node
    long long pathCount = 0;
    
    // Explore all adjacent nodes
    if (graph.find(current) != graph.end()) {
        for (const std::string& neighbor : graph.at(current)) {
            // Only visit if not already visited
            if (visited.find(neighbor) == visited.end()) {
                pathCount += memoizedDFS(graph, neighbor, destination, visited, newVisitedDac, newVisitedFft, memo);
            }
        }
    }
    
    // Backtrack: remove current node from visited set
    visited.erase(current);
    
    // Memoize the result
    memo[key] = pathCount;
    return pathCount;
}

// Part 1 solution
void solvePart1(std::istream& input) {
    std::string line;
    Graph graph;
    
    // Parse input to build the graph
    while (std::getline(input, line)) {
        // Split the line at ':'
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string source = line.substr(0, colonPos);
            std::string targets = line.substr(colonPos + 1);
            
            // Parse the targets (devices this device connects to)
            std::istringstream targetStream(targets);
            std::string target;
            
            // Initialize the source node's adjacency list if it doesn't exist
            if (graph.find(source) == graph.end()) {
                graph[source] = std::vector<std::string>();
            }
            
            // Add each target to the source's adjacency list
            while (targetStream >> target) {
                graph[source].push_back(target);
                
                // Initialize the target node's adjacency list if it doesn't exist
                if (graph.find(target) == graph.end()) {
                    graph[target] = std::vector<std::string>();
                }
            }
        }
    }
    
    // Count paths from "you" to "out"
    int pathCount = 0;
    std::set<std::string> visited;
    dfs(graph, "you", "out", visited, pathCount);
    
    std::cout << "\nPart 1 Answer: " << pathCount << std::endl;
}

// Part 2 solution
void solvePart2(std::istream& input) {
    std::string line;
    Graph graph;
    
    // Parse input to build the graph
    while (std::getline(input, line)) {
        // Split the line at ':'
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string source = line.substr(0, colonPos);
            std::string targets = line.substr(colonPos + 1);
            
            // Parse the targets (devices this device connects to)
            std::istringstream targetStream(targets);
            std::string target;
            
            // Initialize the source node's adjacency list if it doesn't exist
            if (graph.find(source) == graph.end()) {
                graph[source] = std::vector<std::string>();
            }
            
            // Add each target to the source's adjacency list
            while (targetStream >> target) {
                graph[source].push_back(target);
                
                // Initialize the target node's adjacency list if it doesn't exist
                if (graph.find(target) == graph.end()) {
                    graph[target] = std::vector<std::string>();
                }
            }
        }
    }
    
    // Count paths from "svr" to "out" that visit both "dac" and "fft"
    std::set<std::string> visited;
    std::map<MemoKey, long long> memo;
    
    std::cout << "Starting memoized DFS search for paths...\n";
    long long pathCount = memoizedDFS(graph, "svr", "out", visited, false, false, memo);
    
    std::cout << "Part 2 Answer: " << pathCount << std::endl;
}

int main() {
    // Test with example input for Part 1
    std::cout << "Testing with example input for Part 1:" << std::endl;
    std::ifstream exampleFile("example.in");
    if (exampleFile.is_open()) {
        solvePart1(exampleFile);
        exampleFile.close();
    } else {
        std::cerr << "Could not open example.in" << std::endl;
    }
    
    // Test with example input for Part 2
    std::cout << "\nTesting with example input for Part 2:" << std::endl;
    std::ifstream examplePart2File("example_part2.in");
    if (examplePart2File.is_open()) {
        solvePart2(examplePart2File);
        examplePart2File.close();
    } else {
        std::cerr << "Could not open example_part2.in" << std::endl;
    }
    
    // Solve with actual input
    std::cout << "\nSolving with actual input:" << std::endl;
    std::ifstream inputFile("day11.in");
    if (inputFile.is_open()) {
        solvePart1(inputFile);
        
        // Reset file stream to beginning
        inputFile.clear();
        inputFile.seekg(0);
        
        solvePart2(inputFile);
        inputFile.close();
    } else {
        std::cerr << "Could not open day11.in" << std::endl;
    }
    
    return 0;
}
