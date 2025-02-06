#include <stdio.h>
#include "navigate.h"
#include "globals.h"
#include "algorithm_structs_PUBLIC/Path.h"
#include "track_files_PRIVATE/track_navigation.h"

// Function to rotate the car to a given direction
void rotate_to(Direction target_direction) {

        current_car.current_orientation = target_direction;



}


// Function to navigate the car along a given path
void navigate_path(const Path *p) {
    if (!p || !p->route || p->totalDistance == 0) {
        printf("❌ Invalid path! Cannot navigate.\n");
        return;
    }

    printf("\n🚗 Starting Navigation Along the Path...\n");

    // Iterate through each step in the path
    for (int i = 0; i<p->totalDistance; i++) {
        FundamentalPath *step = p->route[i];

        if (current_car.current_location.x == p->end->location.x && current_car.current_location.x == p->end->location.x) {
            break;
        }
        if (!step || !step->end) {
            printf("❌ Invalid step detected! Stopping navigation.\n");
            return;
        }

        printf("➡️ Moving from ID %d (%d, %d) to ID %d (%d, %d) via %s (Distance: %d)\n",
               step->start->id, step->start->location.x, step->start->location.y,
               step->end->id, step->end->location.x, step->end->location.y,
               direction_to_string(step->direction), step->distance);

        // Rotate the car to face the correct direction
        rotate_to(step->direction);

        // Move forward in the correct direction
        for (int j = 0; j < step->distance; j++) {
            move_forward();
            print_grid();  // Visualize movement on the grid
        }

        // Update the car's position
        current_car.current_location = step->end->location;
    }

    printf("🏁 Navigation complete! Arrived at destination (%d, %d).\n",
           p->end->location.x, p->end->location.y);
}
