# Last-Mile Delivery Route Optimizer in C++

Package delivery optimizer using shortest paths and assignment algorithms.

This is a console-based C++ self project built for SDE internship preparation. It models a small delivery network with warehouses, delivery stops, roads, vehicles, and packages. The program calculates shortest travel distances using Dijkstra's algorithm and compares four delivery assignment strategies.

I chose this project because I am from a Mechanical Engineering background and wanted to connect transportation/logistics with software development. It helped me practice C++, graphs, file handling, STL containers, and basic optimization thinking.

## Project Snapshot

| Item | Details |
| --- | --- |
| Language | C++20 |
| Project Type | Console-based self project |
| Main Topic | Graphs, shortest paths, assignment logic |
| Input | CSV files |
| Output | Console table, route summary CSV, comparison text file |
| Highlighted Strategy | Hungarian Algorithm |

## Key Results

Using the sample dataset, the Hungarian Algorithm gives the lowest delivery cost and improves over Random Assignment by:

- Distance reduced by `26.5%`
- Cost reduced by `30.6%`
- Delays reduced by `60.0%`

## Project Highlights

- Loads warehouses, delivery stops, roads, vehicles, and packages from CSV files
- Stores the road network using an adjacency list
- Uses Dijkstra's algorithm with a `priority_queue`
- Generates delivery tasks from warehouse to stop
- Compares Random Assignment, Nearest Neighbor, Optimized Greedy, and Hungarian Algorithm
- Highlights Hungarian Algorithm as the main assignment optimization method
- Calculates delivery distance, cost, utilization, delayed deliveries, and runtime
- Saves route-level and summary output files for review
- Keeps the implementation simple enough to explain in an interview

The main comparison is between simple baselines and stronger optimization methods. Optimized Greedy performs well on route distance, while the Hungarian Algorithm gives the lowest delivery cost in the sample dataset.

## Results From Sample Dataset

| Strategy | Total Distance | Cost | Avg Distance/Package | Delivered | Undelivered | Avg Utilization | Delayed |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Random | 132.0 km | Rs. 2443 | 6.29 km | 21 / 22 | 1 | 85.6% | 5 |
| Nearest Neighbor | 96.0 km | Rs. 1765 | 4.57 km | 21 / 22 | 1 | 83.3% | 2 |
| Optimized Greedy | 94.0 km | Rs. 1749 | 4.48 km | 21 / 22 | 1 | 87.5% | 2 |
| Hungarian Algorithm | 97.0 km | Rs. 1695 | 4.62 km | 21 / 22 | 1 | 87.5% | 2 |

Note: these comparisons are accurate for the simplified model used in this project. Optimized Greedy gives the shortest route distance in the sample data, while the Hungarian Algorithm gives the lowest delivery cost because it is built around the assignment cost table.

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
|   |-- Graph.cpp
|   |-- CsvReader.cpp
|   |-- RoutePlanner.cpp
|   |-- VehicleAssigner.cpp
|   `-- Metrics.cpp
|
|-- include/
|   |-- Graph.h
|   |-- CsvReader.h
|   |-- RoutePlanner.h
|   |-- VehicleAssigner.h
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

## Dataset Format

### warehouses.csv

```csv
warehouse_id,node_id
W1,0
W2,8
```

### stops.csv

```csv
stop_id,node_id
S1,3
S2,4
S3,5
```

### roads.csv

```csv
from_node,to_node,distance_km
0,1,4
0,2,6
1,3,5
```

### vehicles.csv

```csv
vehicle_id,warehouse_id,capacity,cost_per_km,max_distance_per_day
V1,W1,5,18,28
V2,W1,4,22,22
```

### packages.csv

```csv
package_id,warehouse_id,stop_id,priority
PK1,W1,S1,3
PK2,W1,S2,2
```

## How To Compile

Run this from inside the project folder:

```bash
g++ -std=c++20 src/main.cpp src/Graph.cpp src/CsvReader.cpp src/RoutePlanner.cpp src/VehicleAssigner.cpp src/Metrics.cpp -Iinclude -o delivery_optimizer
```

On Windows, the easiest option is to double-click:

```text
build_and_run.bat
```

This file builds the project and then runs it from the correct folder.

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

If the console window closes immediately on Windows, double-click:

```text
run_existing.bat
```

That runs the existing `.exe` and keeps the window open.

## If It Does Not Run

Common fixes:

- Make sure you are inside the `Last_Mile_Delivery_Route_Optimizer` folder before running commands.
- If `g++ is not recognized`, install MinGW/MSYS2 and add `g++` to PATH.
- If it says input data is missing, run the program from the project root folder, not from inside `src`.
- If double-clicking the `.exe` closes too fast, use `run_existing.bat`.

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

`comparison_summary.txt` contains the results for all strategies, improvement percentages, and a short explanation of why the Hungarian Algorithm is useful for assignment.

## Metrics Calculated

- Total delivery distance
- Total delivery cost
- Average distance per package
- Packages delivered
- Undelivered packages
- Average vehicle utilization
- Delayed deliveries
- Runtime in milliseconds
- Distance improvement percentage
- Cost improvement percentage
- Delay reduction percentage

## HR / Interview Explanation

This project is a practical C++ problem based on delivery routing. I represented the city as a graph because roads naturally connect locations. Warehouses and delivery stops are nodes, and road distances are edge weights.

I used Dijkstra's algorithm because all road distances are non-negative, and I needed shortest paths between warehouse/vehicle locations and delivery stops.

I compared four strategies to show measurable improvement:

- Random assignment gives a weak baseline
- Nearest neighbor improves by choosing nearby stops
- Optimized greedy improves further by considering distance, cost, vehicle capacity, priority, and delay risk
- Hungarian Algorithm treats each vehicle capacity slot as an assignment option and minimizes the total assignment cost

The Hungarian Algorithm is highlighted because it is a standard assignment algorithm and gives the lowest delivery cost in the sample run. The project is small enough to explain clearly, but it still includes real algorithmic concepts and measurable output.

## Time Complexity

Let:

- `V` = number of road nodes
- `E` = number of roads
- `P` = number of packages
- `K` = number of vehicles

Main parts:

- Dijkstra from a node: `O((V + E) log V)`
- Route distance lookup uses cached Dijkstra results
- Random assignment: roughly `O(P * K)`
- Nearest neighbor: roughly `O(K * P^2)` in this simple implementation
- Optimized greedy: roughly `O(P * K * P)` because it repeatedly checks available package-vehicle pairs
- Hungarian Algorithm: about `O(N^3)`, where `N` is the larger of package count and vehicle capacity slots

This is fine for a small learning project. A real logistics system would need more advanced routing methods.

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

## Resume Bullets

**Last-Mile Delivery Route Optimizer in C++ | Self Project**  
Package delivery optimizer using shortest paths and assignment algorithms

- Modeled warehouses, delivery stops, and roads as a weighted graph using adjacency lists
- Used Dijkstra's algorithm to compute shortest travel distances between hubs and delivery locations
- Assigned packages/routes to delivery vehicles using greedy assignment based on capacity, distance, and cost
- Compared random, nearest-neighbor, optimized greedy, and Hungarian Algorithm based assignment strategies
- Generated route summaries including total distance, vehicle utilization, delivery cost, and delayed deliveries
