#!/bin/bash

# Function to organize files for a specific day
organize_day() {
    local day=$1
    local day_dir="day$day"
    
    # Create directory if it doesn't exist
    mkdir -p "$day_dir"
    
    # Move files to day directory
    if [ -f "day$day.cpp" ]; then
        mv "day$day.cpp" "$day_dir/"
        echo "Moved day$day.cpp to $day_dir/"
    fi
    
    if [ -f "day$day.in" ]; then
        mv "day$day.in" "$day_dir/"
        echo "Moved day$day.in to $day_dir/"
    fi
}

# Remove binary files
remove_binaries() {
    # Find and remove executable files
    find . -type f -executable -not -name "*.sh" -exec rm {} \; -exec echo "Removed binary: {}" \;
    
    # Remove specific binary files if they exist
    for bin in a.out day*[0-9]; do
        if [ -f "$bin" ]; then
            rm "$bin"
            echo "Removed binary: $bin"
        fi
    done
}

# Move example files to their respective day directories
move_example_files() {
    # If there's only one example.in file and day1 directory exists, move it there
    if [ -f "example.in" ] && [ -d "day1" ]; then
        mv "example.in" "day1/"
        echo "Moved example.in to day1/"
    fi
    
    # For other example files that might be named differently
    for example in example*.in; do
        if [ -f "$example" ] && [[ "$example" =~ example([0-9]+).in ]]; then
            day=${BASH_REMATCH[1]}
            if [ -d "day$day" ]; then
                mv "$example" "day$day/"
                echo "Moved $example to day$day/"
            fi
        fi
    done
}

# Main execution
echo "Organizing files into day-wise structure..."

# Organize existing days
for day in {1..25}; do
    if [ -f "day$day.cpp" ] || [ -f "day$day.in" ]; then
        organize_day $day
    fi
done

# Move example files
move_example_files

# Remove binary files
remove_binaries

echo "Organization complete!"
