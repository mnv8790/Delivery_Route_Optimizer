# Delivery Assignment Optimizer in C++

A console-based C++20 self project that compares package-to-vehicle assignment methods using shortest paths and basic optimization algorithms.

The project uses a small sample delivery network with warehouses, stops, roads, vehicles, and packages. It is not a full logistics system or a complete Vehicle Routing Problem solver. The main focus is assignment comparison: how different strategies affect delivery distance, cost, vehicle use, delayed deliveries, and runtime.

## Why I Built This

I built this project after studying the Hungarian Algorithm in Operations Research. Since I am from Mechanical Engineering, I wanted to connect OR concepts with C++ programming and data structures.

This project helped me practice C++, graphs, Dijkstra's algorithm, file handling, STL containers, and assignment algorithms in one small but measurable project.

## What The Project Does

- Loads roads, warehouses, delivery stops, vehicles, and packages from CSV files
- Stores the road network as a weighted graph using an adjacency list
- Uses Dijkstra's algorithm to calculate shortest travel distances
- Assigns packages to vehicles using different methods
- Compares Random Assignment, Nearest Neighbor, Optimized Greedy, and Hungarian Algorithm
- Prints a comparison summary in the console
- Saves route and summary files inside the `outputs/` folder

The dataset is intentionally small so the logic is easy to understand and explain in an interview.

## Features

- C++20 console application
- Graph representation using adjacency list
- Dijkstra's shortest path algorithm with `priority_queue`
- CSV input and output
- Vehicle capacity based assignment
- Runtime and delivery metric comparison
- User input option to add one extra package before running the comparison
- Hungarian Algorithm highlighted as the cost-focused optimized method

## Algorithms Used

### Dijkstra's Algorithm

The city road network is stored as a weighted graph. Dijkstra's algorithm is used to find the shortest distance between warehouses and delivery stops.

### Random Assignment

This is used as a baseline. Packages are assigned randomly to available vehicles, so the result gives a rough comparison point.

### Nearest Neighbor

This strategy chooses closer deliveries first. It is simple and usually better than random assignment, but it does not always give the best overall result.

### Optimized Greedy

This method uses distance, cost, capacity, package priority, and delay penalty while choosing assignments. It is still greedy, so it makes locally good choices instead of checking every possible assignment.

### Hungarian Algorithm

The Hungarian Algorithm is the main algorithm I wanted to implement in this project. It is used to solve an assignment cost table and find a lower-cost matching between packages and available vehicle capacity slots.

In the sample dataset, the Hungarian Algorithm gives the lowest delivery cost. Optimized Greedy gives the shortest total distance, which is a useful reminder that "best" depends on the metric being compared.

## Sample Results

These results are from the included sample CSV files. Runtime can change slightly depending on the computer.

| Strategy | Total Distance | Cost | Delivered | Undelivered | Avg Utilization | Delayed | Runtime |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Random Assignment | 132.0 km | Rs. 2443 | 21 / 22 | 1 | 85.6% | 5 | 0.180 ms |
| Nearest Neighbor | 96.0 km | Rs. 1765 | 21 / 22 | 1 | 83.3% | 2 | 0.067 ms |
| Optimized Greedy | 94.0 km | Rs. 1749 | 21 / 22 | 1 | 87.5% | 2 | 0.177 ms |
| Hungarian Algorithm | 97.0 km | Rs. 1695 | 21 / 22 | 1 | 87.5% | 2 | 0.244 ms |

Improvement over Random Assignment using Hungarian Algorithm:

- Distance reduced by `26.5%`
- Cost reduced by `30.6%`
- Delayed deliveries reduced by `60.0%`

## Folder Structure

```text
Delivery-Assignment-Optimizer/
|-- src/
|   |-- main.cpp
|   |-- ConsoleHelper.cpp
|   |-- Graph.cpp
|   |-- CsvReader.cpp
|   |-- RoutePlanner.cpp
|   |-- VehicleAssigner.cpp
|   |-- HungarianSolver.cpp
|   `-- Metrics.cpp
|
|-- include/
|   |-- ConsoleHelper.h
|   |-- Graph.h
|   |-- CsvReader.h
|   |-- RoutePlanner.h
|   |-- VehicleAssigner.h
|   |-- HungarianSolver.h
|   `-- Metrics.h
|
|-- data/
|   |-- warehouses.csv
|   |-- stops.csv
|   |-- roads.csv
|   |-- vehicles.csv
|   `-- packages.csv
|
|-- outputs/
|   |-- route_summary.csv
|   `-- comparison_summary.txt
|
|-- build_and_run.bat
`-- README.md
```

## How To Compile And Run

Run this from inside the project folder:

```bash
g++ -std=c++20 src/main.cpp src/ConsoleHelper.cpp src/Graph.cpp src/CsvReader.cpp src/RoutePlanner.cpp src/VehicleAssigner.cpp src/HungarianSolver.cpp src/Metrics.cpp -Iinclude -o delivery_optimizer
```

On Windows, the easiest option is to double-click:

```text
build_and_run.bat
```

This builds the project and then runs it from the correct folder.

On Windows PowerShell:

```bash
.\delivery_optimizer.exe
```

On Linux or macOS:

```bash
./delivery_optimizer
```

## Program Input

When the program starts, it asks:

```text
1. Run comparison with CSV input
2. Change input by adding one package, then run comparison
0. Exit
```

Option `1` runs the comparison using the existing CSV files. Option `2` lets the user enter one extra package from the console and then runs the comparison again.

## Output Files

The program writes two files:

- `outputs/route_summary.csv`
- `outputs/comparison_summary.txt`

`route_summary.csv` contains rows like:

```csv
strategy,vehicle_id,warehouse_id,stop_id,package_count,distance_km,cost,utilization_percent,status
Random,V1,W1,S1,1,9.00,162.00,100.0,DELIVERED
```

`comparison_summary.txt` contains the results for all strategies and improvement percentages.

## What I Learned

- How to model a transportation-style problem using graphs
- How Dijkstra's algorithm works with a priority queue
- Why greedy methods are simple but not always globally optimal
- How the Hungarian Algorithm can be used for cost-based assignment
- How to compare algorithms using measurable metrics
- How to organize a C++ project with multiple source and header files

## Limitations

- This project is tested on a small sample dataset
- Roads are treated as two-way roads
- CSV parsing assumes clean input
- Package size and weight are not deeply modeled
- Vehicles do not return to the warehouse
- Traffic, time windows, and driver shifts are not included
- This is not a full vehicle routing problem solver

## Future Improvements

- Add package weight and volume
- Add delivery time windows
- Add vehicle return-to-warehouse distance
- Use real map coordinates
- Try larger random datasets
- Add better route visualization
- Compare with a more advanced optimization method later
