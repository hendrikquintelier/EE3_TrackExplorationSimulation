#include <stdio.h>
#include <stdlib.h>
#include "Path.h"

// Initialize a Path
void initialize_path(Path *path, MapPoint *start, MapPoint *end) {
    if (!path || !start || !end) {
        perror("Null pointer passed to initialize_path");
        return;
    }

    path->start = start;
    path->end = end;
    path->totalDistance = 0;  // Default total distance

    // Find a route between start and end (not implemented yet)
    path->route = NULL;  // Placeholder for actual path-finding algorithm

    printf("Initialized Path from MapPoint %d to MapPoint %d\n", start->id, end->id);
}

// Print a Path
// Function to print a Path result
void printPathResult(const Path *path) {
    if (!path || !path->start || !path->end) {
        printf("❌ No valid path found.\n");
        return;
    }

    printf("\n===== Path Information =====\n");
    printf("Start MapPoint: ID %d, Location: (%d, %d)\n",
           path->start->id, path->start->location.x, path->start->location.y);

    printf("End MapPoint (Target): ID %d, Location: (%d, %d)\n",
           path->end->id, path->end->location.x, path->end->location.y);

    printf("Total Distance: %d\n", path->totalDistance);

    printf("================================\n");
}
