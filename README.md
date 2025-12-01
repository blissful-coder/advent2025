# Advent of Code 2025

This repository contains my solutions for the Advent of Code 2025 challenges.

## Directory Structure

Each day's challenge is organized in its own directory:

```
day1/
  ├── day1.cpp  # Solution code
  ├── day1.in   # Input data
  └── example.in # Example input data
day2/
  ├── day2.cpp
  ├── day2.in
  └── example.in
...
```

## Scripts

### Initialize a New Day

The `init_day.sh` script creates a new directory with template files for a specific day:

```bash
./init_day.sh <day_number>
```

For example:

```bash
./init_day.sh 2  # Initialize day 2
```

This will:
1. Create a `day2` directory
2. Create a template C++ file (`day2.cpp`)
3. Create empty input files (`day2.in` and `example.in`)

### Organize Files

The `organize.sh` script organizes existing files into the day-wise structure and removes binary files:

```bash
./organize.sh
```

This will:
1. Move existing day files (e.g., `day1.cpp`, `day1.in`) into their respective directories
2. Move example files to their respective day directories
3. Remove binary files (e.g., `a.out`, `day1`)

## Compiling and Running

To compile and run a specific day's solution:

```bash
# Compile
g++ -std=c++17 day<N>/day<N>.cpp -o day<N>/day<N>

# Run with example input
./day<N>/day<N> < day<N>/example.in

# Run with actual input
./day<N>/day<N> < day<N>/day<N>.in
```

Replace `<N>` with the day number.
