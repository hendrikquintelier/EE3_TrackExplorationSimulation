#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../globals.h"
#include "MapPoint.h"
#include "FundamentalPath.h"
#include "../direction.h"

// Static counter for unique MapPoint IDs
static int map_point_counter = 0;

// Function to create a new Map Point
void initialize_map_point(MapPoint *mp, Location location, bool UltraSonicDetection[3]) {
    if (!mp) {
        perror("Null pointer passed to initialize_map_point");
        return;
    }

    // Assign unique ID and location
    mp->id = map_point_counter++;
    mp->location = location;

    // Determine number of paths based on detected options
    int pathCount = 0;
    for (int i = 0; i < 3; ++i) {
        if (UltraSonicDetection[i]) pathCount++;
    }
    mp->numberOfPaths = pathCount;

    // Allocate memory for paths
    mp->paths = malloc(pathCount * sizeof(FundamentalPath));
    if (!mp->paths) {
        perror("Failed to allocate memory for FundamentalPaths");
        exit(EXIT_FAILURE);
    }

    // Initialize paths and assign directions immediately
    int pathIndex = 0;
    for (int i = 0; i < 3; ++i) {
        if (UltraSonicDetection[i]) {
            initialize_fundamental_path(&mp->paths[pathIndex], mp, 0);

            // Assign the correct direction based on the detected index
            switch (i) {
                case 0: // Forward
                    mp->paths[pathIndex].direction = current_car.current_orientation;
                    break;
                case 1: // Left
                    mp->paths[pathIndex].direction = turn_left(current_car.current_orientation);
                    break;
                case 2: // Right
                    mp->paths[pathIndex].direction = turn_right(current_car.current_orientation);
                    break;
            }

            pathIndex++; // Move to the next available slot
        }
    }

    // Add to global array map_points_all
    if (num_map_points_all == capacity_map_points_all) {
        capacity_map_points_all *= 2;
        map_points_all = realloc(map_points_all, capacity_map_points_all * sizeof(MapPoint));
        if (!map_points_all) {
            perror("Failed to resize map_points_all array");
            exit(EXIT_FAILURE);
        }
    }
    map_points_all[num_map_points_all++] = mp;


    if (mp_has_unexplored_paths(mp)) {
        add_map_point_tbd(mp);
    }
}



// Function to add a MapPoint to the "To Be Discovered" list
void add_map_point_tbd(MapPoint *mp) {
    if (num_map_points_tbd == capacity_map_points_tbd) {
        capacity_map_points_tbd *= 2;
        map_points_tbd = realloc(map_points_tbd, capacity_map_points_tbd * sizeof(MapPoint));
        if (!map_points_tbd) {
            perror("Failed to expand map_points_tbd");
            exit(EXIT_FAILURE);
        }
    }
    if (mp->id != 0) {
        map_points_tbd[num_map_points_tbd++] = mp;
    }
}

// Check if a mappoint has paths with an unknown endpoint
int mp_has_unexplored_paths(MapPoint *mp) {
    for (int i = 0; i < mp->numberOfPaths; i++) {
        if (mp->paths[i].end == NULL) {
            return 1;  // Found an unexplored path
        }
    }
    return 0;
}

// Function that prints the info stored in the map point
void print_map_point(const MapPoint *mp) {
    if (!mp) {
        printf("Error: Null MapPoint passed to print_map_point.\n");
        return;
    }

    printf("\n========== MapPoint Info ==========\n");
    printf("ID: %d\n", mp->id);
    printf("Location: (%d, %d)\n", mp->location.x, mp->location.y);
    printf("Number of Paths: %d\n", mp->numberOfPaths);
    printf("------------------------------------\n");

    for (int i = 0; i < mp->numberOfPaths; ++i) {
        printf("  Path %d -> ", i + 1);

        if (mp->paths[i].end) {  // ✅ Check if 'end' is NULL before accessing 'id'
            printf("Leads to MapPoint ID: %d , distance: %d ", mp->paths[i].end->id, mp->paths[i].distance);
        } else {
            printf("Leads to: Unknown ");
        }

        printf("[Direction: %s]\n", direction_to_string(mp->paths[i].direction));
    }

    printf("====================================\n");
}


// Function to check if a MapPoint with current_location already exists
MapPoint *check_map_point_already_exists() {
    for (int i = 0; i < num_map_points_all; i++) {
        if (map_points_all[i]->location.x == current_car.current_location.x &&
            map_points_all[i]->location.y == current_car.current_location.y) {
            return map_points_all[i];  // Return pointer to the existing MapPoint
            }
    }
    return NULL;  // Return NULL if no MapPoint found
}


int calculate_distance(Location a, Location b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Function to update an existing MapPoint and link it with the most recently added MapPoint
void update_existing_mappoint(MapPoint *existing_point) {
    if (num_map_points_all < 2) {
        printf("Not enough MapPoints to establish a connection.\n");
        return;
    }

    // The latest added MapPoint
    MapPoint *latest_point = map_points_all[num_map_points_all - 1];

    printf("Connecting MapPoint at (%d, %d) with the latest MapPoint at (%d, %d)\n",
           existing_point->location.x, existing_point->location.y,
           latest_point->location.x, latest_point->location.y);

    // Determine the direction and distance
    Direction existing_to_latest = determine_direction(existing_point, latest_point);
    Direction latest_to_existing = opposite_direction(existing_to_latest);
    int distance = calculate_distance(existing_point->location, latest_point->location);



    // Search for existing paths and update if necessary
    int updated = 0;
    for (int i = 0; i < existing_point->numberOfPaths; i++) {
        if (existing_point->paths[i].direction == existing_to_latest) {
            existing_point->paths[i].end = latest_point;
            existing_point->paths[i].distance = distance;
            updated = 1;
            break;
        }
    }

    for (int i = 0; i < latest_point->numberOfPaths; i++) {
        if (latest_point->paths[i].direction == latest_to_existing) {
            latest_point->paths[i].end = existing_point;
            latest_point->paths[i].distance = distance;
            updated = 1;
            break;
        }
    }

    // If no existing path was updated, create new FundamentalPaths
    if (!updated) {
        // Allocate new paths dynamically
        existing_point->paths = realloc(existing_point->paths, (existing_point->numberOfPaths + 1) * sizeof(FundamentalPath));
        latest_point->paths = realloc(latest_point->paths, (latest_point->numberOfPaths + 1) * sizeof(FundamentalPath));

        if (!existing_point->paths || !latest_point->paths) {
            perror("❌ Error: Failed to allocate memory for FundamentalPaths");
            exit(EXIT_FAILURE);
        }

        // Initialize new paths
        initialize_fundamental_path(&existing_point->paths[existing_point->numberOfPaths], existing_point, distance);
        existing_point->paths[existing_point->numberOfPaths].end = latest_point;
        existing_point->paths[existing_point->numberOfPaths].direction = existing_to_latest;
        existing_point->numberOfPaths++;

        initialize_fundamental_path(&latest_point->paths[latest_point->numberOfPaths], latest_point, distance);
        latest_point->paths[latest_point->numberOfPaths].end = existing_point;
        latest_point->paths[latest_point->numberOfPaths].direction = latest_to_existing;
        latest_point->numberOfPaths++;

        printf("✅ Path established between (%d, %d) and (%d, %d) with distance %d\n",
               existing_point->location.x, existing_point->location.y,
               latest_point->location.x, latest_point->location.y,
               distance);
    }
}
