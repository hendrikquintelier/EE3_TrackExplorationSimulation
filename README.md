# EE3 Track Exploration Simulation

🚗 **Autonomous Track Exploration & Navigation Algorithm** 🚗  
*A project by Hendrik Quintelier (2025) – part of my Undergraduate Dissertation*

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
# 🚀 Future Goals: Dual-Core Asynchronous Thread Optimization  

## 🔮 Vision for Advanced Dual-Core Optimization  
The **current system** effectively utilizes a **single-core exploration approach**, ensuring the car efficiently maps the track and optimizes navigation. However, **future improvements** will focus on harnessing the full **dual-core capabilities** of the **ESP32-S3** by implementing **asynchronous multithreading** for real-time responsiveness and **greater efficiency**.

---

## 🎯 Future Enhancements with Dual-Core Asynchronous Processing  
The next evolution of this system will leverage **ESP32-S3's two cores** to execute parallel tasks efficiently. Below is the **future system goal**, where multiple threads will **run asynchronously** for optimal track exploration and navigation.

### 🔀 Parallel Task Distribution  
| **Task**                  | **Core** | **Type** | **Purpose** |
|---------------------------|---------|----------|-------------|
| **Sensor Data Acquisition** | Core 0 | High-Priority Interrupt | Uses **asynchronous sensor fusion** to handle real-time ultrasonic, IMU, and wheel encoder readings. |
| **Motor Control (PID-based)** | Core 1 | High-Priority Task | Implements **PID feedback loops** for fine-tuned movement control. |
| **Navigation Algorithm** | Core 1 | Medium-Priority Task | Handles real-time **decision-making** based on sensor inputs and track mapping. |
| **Data Logging & Communication** | Core 0 | Low-Priority Background Task | **Transmits data over WiFi/Bluetooth** and logs exploration performance. |

---

## 🛠 Key Upgrades: Dual-Core Processing for Real-Time Performance  
The **primary challenge** in the future will be **ensuring non-blocking execution** of sensor inputs while maintaining **precise motor control**. The following improvements will be implemented:

### 1️⃣ Asynchronous Sensor Data Handling  
- Instead of **polling** the ultrasonic sensors and IMU, the system will rely on **hardware interrupts**.
- **Interrupt-Driven Approach**:
  - **Ultrasonic Sensor ISR:** Reads distance data asynchronously.
  - **IMU Data Ready ISR:** Updates yaw angle in real time.
  - **Wheel Encoder ISR:** Updates speed and position tracking.

#### 🚀 Benefits:  
✅ Eliminates **blocking delays** in sensor processing.  
✅ Enables **real-time sensor fusion** without affecting motor performance.  

---

### 2️⃣ PID Motor Control Optimization  
- The **motor control system** will be handled by **Core 1**, operating independently of sensor tasks.
- Uses a **closed-loop PID control** system for **adaptive speed regulation** and **precise turning**.

#### 🚀 Benefits:  
✅ Maintains **stable velocity & smooth turns**.  
✅ Prevents **overcorrection & oscillation** due to drift errors.  

---

### 3️⃣ Navigation & Decision-Making Improvements  
- The **navigation logic** will be executed **independently** on Core 1.
- Utilizes **sensor fusion** to intelligently decide:
  - When to turn or adjust speed.
  - How to optimize the path **without blocking other tasks**.
  - When to switch from **exploration mode** to **optimal lap execution mode**.

#### 🚀 Benefits:  
✅ **Reduces reaction time** in obstacle detection.  
✅ **Optimizes decision-making speed** for track navigation.  

---

### 4️⃣ Data Logging & Remote Communication  
- Core 0 will manage **data logging** and **WiFi/Bluetooth communication** without **disrupting** navigation.
- This enables **real-time tracking & debugging** via a **remote dashboard**.

#### 🚀 Benefits:  
✅ Provides **real-time telemetry** for performance monitoring.  
✅ Allows remote debugging & system adjustments.  

---

## 🌍 Final Vision: A Fully Optimized Dual-Core Racing AI  
By integrating **asynchronous multithreading**, the **future system** will be capable of:  
- **Real-time sensor fusion** without delays.  
- **Non-blocking motor control** for smoother turns.  
- **Predictive navigation** for efficient path execution.  
- **Remote performance tracking** using telemetry.  

🚀 **With these advancements, the system will be able to complete races faster and more efficiently than ever before!** 🚗💨  




