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

    // FORMER -> CURRENT path
    // First check if path is already initialized

    Direction fc_direction = determine_direction(former, current);
    FundamentalPath* fc_pointer_fundamental_path = NULL;
    for (int i = 0; i < former->numberOfPaths; ++i) {
        FundamentalPath* path = &former->paths[i];
        if (fc_direction == path->direction) {
            fc_pointer_fundamental_path = path;
        }
    }

    if (fc_pointer_fundamental_path) {
        fc_pointer_fundamental_path->end = current;
    }

    // Create and initialize path from former -> current if not already initialized
    else {

        FundamentalPath* path_fc = malloc(sizeof(FundamentalPath));
        if (!path_fc) {
            perror("Failed to allocate memory for FundamentalPath");
            exit(EXIT_FAILURE);
        }

        initialize_fundamental_path(path_fc, former, 0);
        path_fc->end = current;
        path_fc->direction = fc_direction;


        // Add the path dynamically to former's path list
        former->paths = realloc(former->paths, (former->numberOfPaths + 1) * sizeof(FundamentalPath));
        if (!former->paths) {
            perror("Failed to reallocate memory for former->paths");
            exit(EXIT_FAILURE);
        }
        former->paths[former->numberOfPaths++] = *path_fc;

    }

    // CURRENT -> FORMER path
    // First check if path is already initialized

    Direction cf_direction = opposite_direction(fc_direction);
    FundamentalPath* cf_pointer_fundamental_path = NULL;
    for (int i = 0; i < current->numberOfPaths; ++i) {
        FundamentalPath* path = &current->paths[i];
        if (cf_direction == path->direction) {
            cf_pointer_fundamental_path = path;
        }
    }

    if (cf_pointer_fundamental_path) {
        cf_pointer_fundamental_path->end = former;
    }

    // Create and initialize path from current -> former if not initialized
    else{
        FundamentalPath* path_cf = malloc(sizeof(FundamentalPath));
        if (!path_cf) {
            perror("Failed to allocate memory for FundamentalPath");
            exit(EXIT_FAILURE);
        }
        initialize_fundamental_path(path_cf, current, 0);
        path_cf->end = former;
        path_cf->direction = cf_direction;

        // Add the path dynamically to current's path list
        current->paths = realloc(current->paths, (current->numberOfPaths + 1) * sizeof(FundamentalPath));
        if (!current->paths) {
            perror("Failed to reallocate memory for current->paths");
            exit(EXIT_FAILURE);
        }
        current->paths[current->numberOfPaths++] = *path_cf;

    }
}
