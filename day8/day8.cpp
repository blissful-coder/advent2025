#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <cmath>
#include <queue>
#include <unordered_map>

// Structure to represent a junction box
struct JunctionBox {
    int x, y, z;
    int id; // Unique identifier for each junction box
};

// Structure to represent a connection between two junction boxes
struct Connection {
    int box1, box2;
    double distance;
    
    // Comparison operator for sorting
    bool operator<(const Connection& other) const {
        return distance > other.distance; // For min-heap (priority queue)
    }
};

std::vector<std::vector<int>> inputValues;
std::vector<JunctionBox> junctionBoxes;

void getinput(){
    std::string line;
    int id = 0;
    while (std::getline(std::cin, line)) {
        std::vector<int> convertedValues;
        std::string token;
        std::stringstream ss(line); // Create a stringstream from the input string
        while (std::getline(ss, token, ',')) { // Read tokens until the delimiter
            convertedValues.push_back(std::stoi(token));
        }
        inputValues.push_back(convertedValues);
        
        // Create a junction box from the input values
        if (convertedValues.size() == 3) {
            JunctionBox box;
            box.x = convertedValues[0];
            box.y = convertedValues[1];
            box.z = convertedValues[2];
            box.id = id++;
            junctionBoxes.push_back(box);
        }
    }
}

// Calculate Euclidean distance between two junction boxes
double calculateDistance(const JunctionBox& box1, const JunctionBox& box2) {
    double dx = box1.x - box2.x;
    double dy = box1.y - box2.y;
    double dz = box1.z - box2.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

// Find the root of a set (for Union-Find algorithm)
int findRoot(std::vector<int>& parent, int x) {
    if (parent[x] != x) {
        parent[x] = findRoot(parent, parent[x]); // Path compression
    }
    return parent[x];
}

// Union two sets (for Union-Find algorithm)
void unionSets(std::vector<int>& parent, std::vector<int>& size, int x, int y) {
    int rootX = findRoot(parent, x);
    int rootY = findRoot(parent, y);
    
    if (rootX != rootY) {
        // Union by size: attach smaller tree under root of larger tree
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }
}
// Part 1 solution
void solvePart1() {
    int numBoxes = junctionBoxes.size();
    
    // Calculate distances between all pairs of junction boxes
    std::priority_queue<Connection> connections;
    for (int i = 0; i < numBoxes; i++) {
        for (int j = i + 1; j < numBoxes; j++) {
            Connection conn;
            conn.box1 = i;
            conn.box2 = j;
            conn.distance = calculateDistance(junctionBoxes[i], junctionBoxes[j]);
            connections.push(conn);
        }
    }
    
    // Initialize Union-Find data structures
    std::vector<int> parent(numBoxes);
    std::vector<int> size(numBoxes, 1);
    for (int i = 0; i < numBoxes; i++) {
        parent[i] = i; // Each junction box starts in its own set
    }
    
    // Connect the closest pairs (10 for example, 1000 for actual input)
    int connectionsToMake;
    if (numBoxes == 20) { // Example input has 20 junction boxes
        connectionsToMake = 10;
    } else {
        connectionsToMake = 1000;
    }
    
    connectionsToMake = std::min(connectionsToMake, (int)connections.size());
    for (int i = 0; i < connectionsToMake; i++) {
        Connection conn = connections.top();
        connections.pop();
        
        // Connect the two junction boxes
        unionSets(parent, size, conn.box1, conn.box2);
    }
    
    // Count the size of each circuit
    std::unordered_map<int, int> circuitSizes;
    for (int i = 0; i < numBoxes; i++) {
        int root = findRoot(parent, i);
        circuitSizes[root]++;
    }
    
    // Find the three largest circuits
    std::vector<int> sizes;
    for (const auto& pair : circuitSizes) {
        sizes.push_back(pair.second);
    }
    
    // Sort in descending order
    std::sort(sizes.begin(), sizes.end(), std::greater<int>());
    
    // Calculate the result (product of the three largest circuit sizes)
    long long result = 1;
    for (int i = 0; i < std::min(3, (int)sizes.size()); i++) {
        result *= sizes[i];
    }
    
    std::cout << "Part 1 Answer: " << result << std::endl;
}

// Part 2 solution
void solvePart2() {
    int numBoxes = junctionBoxes.size();
    
    // Calculate distances between all pairs of junction boxes
    std::priority_queue<Connection> connections;
    for (int i = 0; i < numBoxes; i++) {
        for (int j = i + 1; j < numBoxes; j++) {
            Connection conn;
            conn.box1 = i;
            conn.box2 = j;
            conn.distance = calculateDistance(junctionBoxes[i], junctionBoxes[j]);
            connections.push(conn);
        }
    }
    
    // Initialize Union-Find data structures
    std::vector<int> parent(numBoxes);
    std::vector<int> size(numBoxes, 1);
    for (int i = 0; i < numBoxes; i++) {
        parent[i] = i; // Each junction box starts in its own set
    }
    
    // Connect junction boxes until all are in a single circuit
    int numSets = numBoxes; // Initially, each junction box is in its own set
    int lastBox1 = -1, lastBox2 = -1;
    
    while (numSets > 1 && !connections.empty()) {
        Connection conn = connections.top();
        connections.pop();
        
        int root1 = findRoot(parent, conn.box1);
        int root2 = findRoot(parent, conn.box2);
        
        // If they're not already in the same set, connect them
        if (root1 != root2) {
            unionSets(parent, size, conn.box1, conn.box2);
            numSets--;
            
            // Store the last pair that gets connected
            lastBox1 = conn.box1;
            lastBox2 = conn.box2;
        }
    }
    
    // Calculate the result (product of X coordinates of the last two junction boxes)
    long long result = (long long)junctionBoxes[lastBox1].x * junctionBoxes[lastBox2].x;
    
    std::cout << "Part 2 Answer: " << result << std::endl;
}

int main() {
    // Solve both parts
    getinput();
    solvePart1();
    solvePart2();
    return 0;
}
