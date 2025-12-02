#include <iostream>

// Part 1
int mover(int current, int delta, char direction){
    if (direction == 'L') delta = -delta;
    int result = (current + delta) % 100;
    return (result < 0) ? result + 100 : result;
}

// Part 2
// Returns the new position and counts how many times the dial passes through 0 during rotation
std::pair<int, int> mover0x434C49434B(int current, int delta, char direction){
    if (direction == 'L') delta = -delta;
    
    // Calculate the new position
    int newPosition = (current + delta) % 100;
    if (newPosition < 0) newPosition += 100;
    
    // Special case: if we end at 0, we need to handle differently
    // because we don't want to count passing through 0 if we end at 0
    if (newPosition == 0) {
        // For clockwise rotation (positive delta)
        if (delta > 0) {
            // Count full rotations, but subtract 1 because ending at 0 isn't "passing through"
            int fullRotations = (current + delta) / 100;
            if (fullRotations > 0 && (current + delta) % 100 == 0) {
                fullRotations--;
            }
            return {newPosition, fullRotations};
        } 
        // For counterclockwise rotation (negative delta)
        else if (delta < 0) {
            // Count full rotations
            int fullRotations = (-delta) / 100;
            
            // If we don't have any full rotations but we pass through 0
            // (e.g., from 20 to 0 with L20), don't count it as passing through
            if (fullRotations == 0 && current > 0) {
                return {newPosition, 0};
            }
            
            return {newPosition, fullRotations};
        }
    }
    
    // Normal case: we don't end at 0
    int zerosPassedThrough = 0;
    
    // Count zeros passed through during rotation
    if (delta > 0) {
        // Moving clockwise (positive direction)
        // We pass through 0 when we complete a full rotation
        zerosPassedThrough = (current + delta) / 100;
    } else if (delta < 0) {
        // Moving counterclockwise (negative direction)
        // We pass through 0 when we complete a full rotation
        int fullRotations = (-delta) / 100;
        zerosPassedThrough = fullRotations;
        
        // Check if we pass through 0 with the remaining steps
        int remainingSteps = (-delta) % 100;
        if (current < remainingSteps && current > 0)
            zerosPassedThrough++;
    }
    
    return {newPosition, zerosPassedThrough};
}

int main(){
    char direction;
    int number, part1Count=0, part2Count=0, currentState = 50;
    while(std::cin>>direction>>number){
        // Use the new method that counts zeros passed through during rotation
        auto [newState, zerosPassedThrough] = mover0x434C49434B(currentState, number, direction);
        currentState = newState;
        
        // std::cout<<"Moving:"<<direction<<number<<"->"<<currentState;
        // std::cout<<" (passed through 0: "<<zerosPassedThrough<<" times)\n";
        
        // Count if we end at 0 (for both Part 1 and Part 2)
        if(currentState == 0) {
            part1Count++;
            part2Count++;
            // std::cout<<"  Ended at 0!\n";
        }
        
        // Add zeros passed through during rotation (only for Part 2)
        part2Count += zerosPassedThrough;
    }

    std::cout<<"\nPart 1 Answer (only counting when dial ends at 0): "<<part1Count;
    std::cout<<"\nPart 2 Answer (counting all occurrences of 0): "<<part2Count;
    return 0;
}
