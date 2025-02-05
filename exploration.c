#include "exploration.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "track_files_PRIVATE/track_detection.h"
#include "track_files_PRIVATE/track_navigation.h"
#include "algorithm_structs_PUBLIC/MapPoint.h"
#include "algorithm_structs_PUBLIC/FundamentalPath.h"

// Check if the car is at a MapPoint
int is_map_point() {
    int options = 0;

    if (ultrasonic_sensors[1]) options++; // Can move left
    if (ultrasonic_sensors[2]) options++; // Can move right

    return (options > 0);  // More than one option = MapPoint
}

// Print all MapPoint information for debugging
void print_all_map_points() {
    printf("\n===== MapPoint Debug Info =====\n");
    for (int i = 0; i < num_map_points_all; i++) {
        print_map_point(map_points_all[i]);

        printf("----------------------------------\n");
    }
}

// Decide the next move based on ultrasonic sensors
void decide_next_move() {
     if (ultrasonic_sensors[0]) {
        move_forward(&current_car);
        return;
    }

    // If forward is blocked, turn left or right
    if (ultrasonic_sensors[1]) {
        rotate_left(&current_car);
        move_forward(&current_car);
    } else if (ultrasonic_sensors[2]) {
        rotate_right(&current_car);
        move_forward(&current_car);
    } else {
        // No way forward, turn around
        rotate_right(&current_car);
        rotate_right(&current_car);
    }
}

// Selects a MapPoint from the list to be discovered
MapPoint *select_next_mappoint() {
    if (num_map_points_tbd == 0) return NULL;

    // Choose the first available MapPoint that still needs discovery
    for (int i = 0; i < num_map_points_tbd; i++) {
        if (&map_points_tbd[i] != NULL) {  // Check if the address is valid
            return &map_points_tbd[i];    // Return a pointer to the struct
        }
    }
    return NULL;
}


// Navigate to a specific MapPoint
void navigate_to_mappoint(MapPoint *destination) {
    printf("Navigating to MapPoint at (%d, %d)\n",
           destination->location.x, destination->location.y);

    // Call pathfinding function to reach the selected MapPoint
    navigate_to_point(&current_car, destination->location);
}

// Main function to start automatic exploration
void start_exploration() {
    MapPoint *former_map_point = NULL; // Keep track of the previous MapPoint

    while (1) {
        print_grid(current_car);

        // Update sensor readings before each move
        update_ultrasonic_sensors();
        check_mappoints_tbd();

        // Check if the current position is a MapPoint
        if (is_map_point()) {
            MapPoint *existing_point = check_map_point_already_exists();

            if (existing_point) {
                // If MapPoint exists, ensure paths remain connected
                printf("Revisiting existing MapPoint at (%d, %d)\n",
                       existing_point->location.x, existing_point->location.y);

                update_existing_mappoint(existing_point);

                // Update the FundamentalPath between former and current (if not the first step)
                if (former_map_point) {
                    update_latest_fundamental_path(existing_point, former_map_point);
                }

                // Check for unexplored paths at the current MapPoint
                int unexplored_paths = 0;
                for (int i = 0; i < existing_point->numberOfPaths; i++) {
                    if (existing_point->paths[i].end == NULL) {
                        unexplored_paths++;
                    }
                }

                if (unexplored_paths > 0) {
                    printf("Unexplored paths remain at (%d, %d). Continuing exploration here.\n",
                           existing_point->location.x, existing_point->location.y);
                    decide_next_move();
                } else {
                    printf("No unexplored paths. Selecting a new MapPoint.\n");
                    MapPoint *next_point = select_next_mappoint();
                    if (next_point) {
                        navigate_to_mappoint(next_point);
                    } else {
                        printf("Exploration complete! No more points to be discovered.\n");
                        break;
                    }
                }
            } else {
                // Allocate memory for a new MapPoint
                MapPoint *new_map_point = malloc(sizeof(MapPoint));
                if (!new_map_point) {
                    perror("Failed to allocate memory for MapPoint");
                    exit(EXIT_FAILURE);
                }

                // Set location based on the car's current position
                Location location = {current_car.current_location.x, current_car.current_location.y};

                // Pass the ultrasonic detection array to determine paths
                initialize_map_point(new_map_point, location, ultrasonic_sensors);

                printf("New MapPoint created at (%d, %d)\n", location.x, location.y);

                // If this isn't the start position, create a FundamentalPath BACK to former MapPoint
                if (former_map_point) {
                    update_latest_fundamental_path(new_map_point, former_map_point);
                }

                // Set this as the latest known MapPoint
                former_map_point = new_map_point;
            }
        }

        // Move or turn if necessary
        decide_next_move();

        // Print MapPoint information for debugging
        print_all_map_points();

        // Stop when track is fully explored
        if (num_map_points_tbd == 0 && num_all_fundamental_paths != 0) {
            printf("Exploration complete!\n");
            break;
        }

        usleep(500000);  // Slow down movement for realism
    }
}

