# EE3 Track Exploration Simulation

🚗 **Autonomous Track Exploration & Navigation Algorithm** 🚗  
*A project by Hendrik Quintelier (2025) – Undergraduate Dissertation*

## 📌 Overview

This project is part of an undergraduate dissertation focused on autonomous track exploration and navigation. The program simulates a self-driving car navigating a predefined track, dynamically mapping explored sections and optimizing its route to complete three laps as quickly as possible.

Unlike exhaustive exploration algorithms, this implementation prioritizes efficiency over completeness—it stops once the system detects a valid lap rather than exploring every possible path.

---

## 🔍 How It Works

### 1️⃣ Track Representation
The track is modeled as a grid (`GRID_SIZE x GRID_SIZE`), where:

- `'#'` → Represents track sections  
- `'S'` → Marks the start/finish line  
- `'.'` → Represents empty space  

The car starts at a predefined position and continuously updates its location, making real-time decisions based on ultrasonic sensor inputs.

### 2️⃣ Core Algorithm Components

| **Component**              | **Description** |
|----------------------------|----------------------------------------------------------------|
| **Track Initialization**    | Initializes the grid, creating a closed-loop track. |
| **Ultrasonic Sensor Simulation** | Simulates real-world sensors, detecting obstacles in the forward, left, and right directions. |
| **Decision Making**         | Uses sensor data to determine whether to move forward, turn left, or turn right. |
| **Path Tracking**           | Builds a map of explored locations and connects paths between points. |
| **Dijkstra's Algorithm**    | Finds the shortest path to the best unexplored route when necessary. |
| **Lap Recognition**         | Stops exploration once a valid lap is detected instead of exploring all paths. |

---


## 🏁 Example Track

Below is an example of a predefined track loaded by the system:

```plaintext
. . . . . . . . . . . . . 
. . > # # # # # # . . . . 
. . # . . # . . # . . . . 
. . # . . # # # # . . . . 
. . # . . . . . # . . . . 
. . # # # # . . # # # # . 
. . # . . # . . . # . # . 
. . # # # # # # # # . # . 
. . . . . # . . . # . # . 
. . . . # # # # # # # # . 
. . . . # . . . . . # . . 
. . . . # # # # # # # . . 
. . . . . . . . . . . . .
```

- `>` represents the car's starting position and orientation.
- The system will navigate the track using the defined exploration and optimization methods.

---
## 📂 Project Structure

The project is organized into separate modules for **algorithm structures, track processing, navigation, and utilities**.

### 🔧 Core Files & Their Functions

#### 📁 `algorithm_structs_PUBLIC/`
| File                    | Description |
|-------------------------|----------------------------------------------------------------|
| `FundamentalPath.c`     | Implements core path structure and connections between waypoints. |
| `FundamentalPath.h`     | Header file defining the `FundamentalPath` structure and functions. |
| `MapPoint.c`            | Manages key track intersections for pathfinding. |
| `MapPoint.h`            | Header file defining the `MapPoint` structure. |
| `Path.c`                | Handles full track paths and connections. |
| `Path.h`                | Header file defining the `Path` structure and its operations. |

#### 📁 `track_files_PRIVATE/`
| File                    | Description |
|-------------------------|----------------------------------------------------------------|
| `track_detection.c`     | Simulates ultrasonic sensors to detect available paths. |
| `track_detection.h`     | Header file for `track_detection.c`. |
| `track_generation.c`    | Initializes and generates the track layout. |
| `track_generation.h`    | Header file for `track_generation.c`. |
| `track_navigation.c`    | Handles car movement, rotation, and position tracking. |
| `track_navigation.h`    | Header file for `track_navigation.c`. |

#### 📁 Root Directory (Other Core Files)
| File                    | Description |
|-------------------------|----------------------------------------------------------------|
| `Dijkstra.c`            | Implements Dijkstra’s algorithm for shortest pathfinding. |
| `direction.h`           | Defines car movement directions and related enums. |
| `exploration.c`         | Controls the autonomous exploration process. |
| `exploration.h`         | Header file for `exploration.c`. |
| `globals.c`             | Stores global variables used across multiple modules. |
| `globals.h`             | Header file for `globals.c`. |
| `main.c`                | Entry point of the program, starts the simulation. |
| `navigate.c`            | Guides the car through the grid using precomputed paths. |
| `navigate.h`            | Header file for `navigate.c`. |
| `CMakeLists.txt`        | Build configuration file for CMake. |

---

## 🚗 Exploration Algorithm Flow

The exploration algorithm autonomously navigates the car through a predefined track, mapping key locations (**MapPoints**) and optimizing the route to complete laps efficiently. It follows a structured process:

### 1️⃣ **Initialization**
- **Global variables are initialized** (`initialize_globals()`) to manage MapPoints and paths.
- The **track grid is generated** (`initialize_grid()` and `create_loop_track()`).
- The car's **starting position is recorded** (`start = current_car.current_location`).

### 2️⃣ **Autonomous Exploration**
- The car continuously **reads ultrasonic sensor data** (`update_ultrasonic_sensors()`) to detect available paths (forward, left, right).
- If the car reaches an **intersection or decision point**, it is recorded as a **MapPoint** (`initialize_map_point()`).
- If a new path is detected, it is stored as a **FundamentalPath** (`initialize_fundamental_path()`).
- The car **prioritizes moving forward**, but if blocked, it will **turn left or right** (`decide_next_move()`).

### 3️⃣ **Handling MapPoints**
- If the car revisits a **known MapPoint**, it **updates connections** (`update_existing_mappoint()`).
- If the MapPoint has unexplored paths, it is added to **pending exploration** (`add_map_point_tbd()`).
- If all paths at a location are explored, the car uses **Dijkstra’s Algorithm** to find the shortest route to the next unexplored point (`find_shortest_path_to_mappoint_tbd()`).

### 4️⃣ **Path Optimization & Lap Completion**
- Once the **track layout is sufficiently explored**, the car **stops exploration** and switches to **path optimization** (`navigate_path()`).
- The system **determines the shortest lap route** and **repeats it efficiently**.
- **Exploration ends** when a valid lap is completed.

---

### 🔁 **Key Decision Points**
| Condition | Action |
|-----------|--------|
| Path ahead is clear | Move forward (`move_forward()`) |
| Forward is blocked | Turn left or right (`rotate_left()` / `rotate_right()`) |
| At a MapPoint | Record and check for unexplored paths (`is_map_point()`) |
| No paths left to explore | Use **Dijkstra’s Algorithm** to find the next point |

This structured approach ensures **efficient exploration**, **dynamic pathfinding**, and **adaptive decision-making**, enabling the car to **navigate and optimize laps intelligently**. 🚀  



