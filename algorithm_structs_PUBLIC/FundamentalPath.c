#include <stdio.h>
#include <stdlib.h>
#include "FundamentalPath.h"
#include "MapPoint.h"
#include "../direction.h"
#include "../globals.h"

// Static counter for unique FundamentalPath IDs
static int fundamental_path_counter = 0;

// Function to determine the direction from `start` to `end`
Direction determine_direction(MapPoint *start, MapPoint *end) {
    if (!start || !end) return NORTH;  // Default safety value

    if (end->location.x > start->location.x) return EAST;
    if (end->location.x < start->location.x) return WEST;
    if (end->location.y > start->location.y) return SOUTH;
    if (end->location.y < start->location.y) return NORTH;

    return NORTH;  // Should never happen (same location)
}





// Function to determine the straight-line distance of a FundamentalPath
int determine_distance_mappoints(MapPoint *start, MapPoint *end) {
    if (!start || !end) {
        fprintf(stderr, "Error: Null MapPoint passed to determine_distance_fundamentalpath\n");
        return -1;  // Return -1 to indicate an error
    }

    // Calculate the difference in x and y coordinates
    int delta_x = abs(end->location.x - start->location.x);
    int delta_y = abs(end->location.y - start->location.y);

    // Determine direction from start to end
    Direction direction = determine_direction(start, end);

    // Ensure the movement is in a straight line
    if ((direction == NORTH || direction == SOUTH) && delta_x != 0) {
        fprintf(stderr, "Error: Invalid FundamentalPath, expected vertical movement but found horizontal displacement\n");
        return -1; // Error
    }

    if ((direction == EAST || direction == WEST) && delta_y != 0) {
        fprintf(stderr, "Error: Invalid FundamentalPath, expected horizontal movement but found vertical displacement\n");
        return -1; // Error
    }

    // Return the Manhattan distance (since we assume straight lines)
    return delta_x + delta_y;
}

int determine_distance_fundamentalpath(FundamentalPath *path) {
    return determine_distance_mappoints(path->start,path->end);
}


// Initialize a FundamentalPath
void initialize_fundamental_path(FundamentalPath *fp, MapPoint *start, float distance) {
    if (!fp) {
        perror("Null pointer passed to initialize_fundamental_path");
        return;
    }

    fp->id = fundamental_path_counter++;
    fp->start = start;
    fp->end = NULL;
    fp->distance = distance;
    fp->direction = NORTH;  // Default direction, updated later

    add_fundamental_path(fp);

    printf("Initialized FundamentalPath ID: %d\n", fp->id);
}

// Initialize multiple FundamentalPaths based on UltraSonicDetection
FundamentalPath* initialize_fundamental_paths(bool UltraSonicDetection[3]) {
    int pathCount = 0;
    for (int i = 0; i < 3; ++i) {
        if (UltraSonicDetection[i]) pathCount++;
    }

    FundamentalPath* paths = malloc(pathCount * sizeof(FundamentalPath));
    if (!paths) {
        perror("Failed to allocate memory for FundamentalPaths");
        exit(EXIT_FAILURE);
    }

    int pathIndex = 0;
    for (int i = 0; i < 3; ++i) {
        if (UltraSonicDetection[i]) {
            initialize_fundamental_path(&paths[pathIndex++], NULL, 0);
        }
    }

    return paths;
}

// Update fundamental paths when discovering a new MapPoint
void update_latest_fundamental_path(MapPoint* current, MapPoint* former) {
    if (!current || !former) {
        fprintf(stderr, "Null pointer passed to update_latest_fundamental_path\n");
        return;
    }

    // Determine the direction from former to current
    Direction fc_direction = determine_direction(former, current);
    Direction cf_direction = opposite_direction(fc_direction);

    FundamentalPath* fc_pointer_fundamental_path = NULL;
    FundamentalPath* cf_pointer_fundamental_path = NULL;

    // Check if the path from former -> current already exists
    for (int i = 0; i < former->numberOfPaths; ++i) {
        if (former->paths[i].direction == fc_direction) {
            fc_pointer_fundamental_path = &former->paths[i];
            break;
        }
    }

    // If path exists, update its endpoint and distance
    if (fc_pointer_fundamental_path) {
        fc_pointer_fundamental_path->end = current;
        fc_pointer_fundamental_path->distance = determine_distance_fundamentalpath(fc_pointer_fundamental_path);
    }
    else { // Otherwise, create and add a new path
        fc_pointer_fundamental_path = malloc(sizeof(FundamentalPath));
        if (!fc_pointer_fundamental_path) {
            perror("Failed to allocate memory for FundamentalPath");
            exit(EXIT_FAILURE);
        }

        initialize_fundamental_path(fc_pointer_fundamental_path, former, 0);
        fc_pointer_fundamental_path->end = current;
        fc_pointer_fundamental_path->direction = fc_direction;
        fc_pointer_fundamental_path->distance = determine_distance_fundamentalpath(fc_pointer_fundamental_path);

        // Dynamically add to former's path list
        former->paths = realloc(former->paths, (former->numberOfPaths + 1) * sizeof(FundamentalPath));
        if (!former->paths) {
            perror("Failed to reallocate memory for former->paths");
            exit(EXIT_FAILURE);
        }
        former->paths[former->numberOfPaths++] = *fc_pointer_fundamental_path;
    }

    // Check if the path from current -> former already exists
    for (int i = 0; i < current->numberOfPaths; ++i) {
        if (current->paths[i].direction == cf_direction) {
            cf_pointer_fundamental_path = &current->paths[i];
            break;
        }
    }

    // If path exists, update its endpoint and distance
    if (cf_pointer_fundamental_path) {
        cf_pointer_fundamental_path->end = former;
        cf_pointer_fundamental_path->distance = fc_pointer_fundamental_path->distance; // Copy distance
    }
    else { // Otherwise, create and add a new path
        cf_pointer_fundamental_path = malloc(sizeof(FundamentalPath));
        if (!cf_pointer_fundamental_path) {
            perror("Failed to allocate memory for FundamentalPath");
            exit(EXIT_FAILURE);
        }

        initialize_fundamental_path(cf_pointer_fundamental_path, current, 0);
        cf_pointer_fundamental_path->end = former;
        cf_pointer_fundamental_path->direction = cf_direction;
        cf_pointer_fundamental_path->distance = fc_pointer_fundamental_path->distance; // Copy distance

        // Dynamically add to current's path list
        current->paths = realloc(current->paths, (current->numberOfPaths + 1) * sizeof(FundamentalPath));
        if (!current->paths) {
            perror("Failed to reallocate memory for current->paths");
            exit(EXIT_FAILURE);
        }
        current->paths[current->numberOfPaths++] = *cf_pointer_fundamental_path;
    }
}

