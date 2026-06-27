# Delivery Assignment Optimizer in C++

Package delivery optimizer using shortest paths and assignment algorithms.

This is a console-based C++ project that models a small delivery network with warehouses, delivery stops, roads, vehicles, and packages. The program calculates shortest travel distances using Dijkstra's algorithm and compares different delivery assignment strategies.

The main motivation behind this project was to implement the Hungarian Algorithm after studying it in Operations Research and apply it to a practical assignment problem. Along with that, the project helped me strengthen DSA concepts such as graphs, priority queues, shortest paths, greedy approaches, and optimization-based decision making.


## Features

- Loads warehouses, delivery stops, roads, vehicles, and packages from CSV files
- Stores the road network using an adjacency list
- Uses Dijkstra's algorithm with a `priority_queue`
- Generates delivery tasks from warehouse to stop
- Compares Random Assignment, Nearest Neighbor, Optimized Greedy, and Hungarian Algorithm
- Calculates delivery distance, cost, utilization, delayed deliveries, and runtime
- Saves route-level and summary output files for review

Optimized Greedy performs well on route distance, while the Hungarian Algorithm gives the lowest delivery cost in the sample dataset.

## Sample Results

| Strategy | Total Distance | Cost | Delivered | Undelivered | Avg Utilization | Delayed |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Random | 132.0 km | Rs. 2443 | 21 / 22 | 1 | 85.6% | 5 |
| Nearest Neighbor | 96.0 km | Rs. 1765 | 21 / 22 | 1 | 83.3% | 2 |
| Optimized Greedy | 94.0 km | Rs. 1749 | 21 / 22 | 1 | 87.5% | 2 |
| Hungarian Algorithm | 97.0 km | Rs. 1695 | 21 / 22 | 1 | 87.5% | 2 |

Compared to Random Assignment, the Hungarian Algorithm reduces distance by `26.5%`, cost by `30.6%`, and delays by `60.0%`.

## Algorithms Used

- Graph adjacency list for storing roads
- Dijkstra's shortest path algorithm for travel distance
- Random assignment baseline for comparison
- Nearest-neighbor routing for a simple improvement
- Optimized greedy assignment using capacity, cost, distance, priority, and delay penalty
- Hungarian Algorithm using vehicle capacity slots as assignment choices
- Basic metric calculation using STL containers

## Tech Stack

- C++20
- STL containers: `vector`, `unordered_map`, `priority_queue`
- File handling using CSV input/output
- Modular code with headers and source files
- Console-based output

## Folder Structure

```text
Last_Mile_Delivery_Route_Optimizer/
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
`-- README.md
```

## How To Compile

Run this from inside the project folder:

```bash
g++ -std=c++20 src/main.cpp src/ConsoleHelper.cpp src/Graph.cpp src/CsvReader.cpp src/RoutePlanner.cpp src/VehicleAssigner.cpp src/HungarianSolver.cpp src/Metrics.cpp -Iinclude -o delivery_optimizer
```

On Windows, the easiest option is to double-click:

```text
build_and_run.bat
```

This builds the project and then runs it from the correct folder.

## How To Run

On Linux or macOS:

```bash
./delivery_optimizer
```

On Windows PowerShell:

```bash
.\delivery_optimizer.exe
```

When the project starts, it asks for input:

```text
1. Run comparison with CSV input
2. Change input by adding one package, then run comparison
0. Exit
```

Choose `1` to run the dataset from the CSV files. Choose `2` to enter one extra package from the keyboard and then run the comparison again.

## Sample Output

The exact runtime may change slightly on each run.

```text
Last-Mile Delivery Route Optimizer
----------------------------------
1. Run comparison with CSV input
2. Change input by adding one package, then run comparison
0. Exit
Enter your choice: 1

Shortest paths + vehicle assignment comparison
Hungarian Algorithm is highlighted as the lowest-cost assignment method.
Packages loaded: 22
Vehicles loaded: 6

Comparison Summary
Strategy             Distance   Cost(Rs)   Avg/Package   Delivered   Undelivered   Util(%)   Delayed   Runtime(ms)
-----------------------------------------------------------------------------------------------------------------
Random                 132.0       2443          6.29          21             1      85.6         5        0.180
Nearest Neighbor        96.0       1765          4.57          21             1      83.3         2        0.067
Optimized Greedy        94.0       1749          4.48          21             1      87.5         2        0.177
Hungarian Algorithm     97.0       1695          4.62          21             1      87.5         2        0.244

Improvement over Random using Hungarian Algorithm
Distance reduced by: 26.5%
Cost reduced by: 30.6%
Delays reduced by: 60.0%
```

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

- How to model a real-world transportation problem using graphs
- How Dijkstra's algorithm is used for shortest path calculation
- How different assignment strategies affect total cost and distance
- How to organize a C++ project using multiple source and header files
- How to read and write CSV files in C++
- How to compare algorithms using measurable metrics

## Limitations

- Roads are treated as two-way roads
- CSV parsing assumes clean input
- Package size/weight is not modeled
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
