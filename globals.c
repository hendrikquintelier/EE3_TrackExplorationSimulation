#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "direction.h"
#include "track_files_PRIVATE/track_navigation.h"

// Dynamic global arrays
MapPoint **map_points_tbd = NULL;
MapPoint **map_points_all = NULL;
FundamentalPath *all_fundamental_paths = NULL;

// Define global car
Car current_car = {{4, 1}, SOUTH};  // Start at (4,1), facing down

// Define global ultrasonic sensors
bool ultrasonic_sensors[3] = {true, true, true};  // {forward, left, right}

// Sizes and capacities
int num_map_points_tbd = 0, capacity_map_points_tbd = 20;
int num_map_points_all = 0, capacity_map_points_all = 80;
int num_all_fundamental_paths = 0, capacity_all_fundamental_paths = 160;

void initialize_globals() {
    map_points_tbd = malloc(capacity_map_points_tbd * sizeof(MapPoint));
    map_points_all = malloc(capacity_map_points_all * sizeof(MapPoint));
    all_fundamental_paths = malloc(capacity_all_fundamental_paths * sizeof(FundamentalPath));

    if (!map_points_tbd || !map_points_all || !all_fundamental_paths) {
        perror("Failed to allocate global arrays");
        exit(EXIT_FAILURE);
    }
}

void free_globals() {
    free(map_points_tbd);
    free(map_points_all);
    free(all_fundamental_paths);
}



