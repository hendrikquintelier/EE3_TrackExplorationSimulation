#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // For Windows key input
#include <unistd.h> // For usleep (smooth screen updates)
#include "track_navigation.h"
#include "../direction.h"


// Function to print the grid with the car's position
void print_grid() {
    // Clear screen properly
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J"); // ANSI escape code for clearing screen (Linux/macOS)
#endif

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == current_car.current_location.y && j == current_car.current_location.x) {
                printf("%c ", direction_to_symbol(current_car.current_orientation));  // Show car's direction
            } else {
                printf("%c ", grid[i][j]);
            }
        }
        printf("\n");
    }

    fflush(stdout);  // Force immediate output update
    usleep(50000);   // Add small delay to prevent glitches
}

// Function to rotate the car left
void rotate_left() {
    current_car.current_orientation = turn_left(current_car.current_orientation);
}

// Function to rotate the car right
void rotate_right() {
    current_car.current_orientation = turn_right(current_car.current_orientation);
}

// Function to move the car forward if possible
void move_forward() {
    int new_x = current_car.current_location.x;
    int new_y = current_car.current_location.y;

    // Determine next position based on direction
    switch (current_car.current_orientation) {
        case NORTH: new_y -= 1; break;
        case SOUTH: new_y += 1; break;
        case WEST:  new_x -= 1; break;
        case EAST:  new_x += 1; break;
    }

    // Only move if the next position is part of the track
    if (grid[new_y][new_x] == TRACK || grid[new_y][new_x] == START_FINISH) {
        current_car.current_location.x = new_x;
        current_car.current_location.y = new_y;
    }
}

// Helper function to calculate Manhattan Distance
int calculate_distance(Location a, Location b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Function to move towards a target location
void navigate_to_point(Car *car, Location target_location) {
    printf("Navigating from (%d, %d) to (%d, %d)\n",
           car->current_location.x, car->current_location.y,
           target_location.x, target_location.y);

    // While the car has not reached the destination
    while (car->current_location.x != target_location.x || car->current_location.y != target_location.y) {

        // Update sensor readings before each move
        update_ultrasonic_sensors();

        // Determine the best move based on current position and target
        if (car->current_location.x < target_location.x && ultrasonic_sensors[0]) {
            move_forward();
        } else if (car->current_location.x > target_location.x && ultrasonic_sensors[0]) {
            move_forward();
        } else if (car->current_location.y < target_location.y && ultrasonic_sensors[1]) {
            rotate_left();
            move_forward();
        } else if (car->current_location.y > target_location.y && ultrasonic_sensors[2]) {
            rotate_right();
            move_forward();
        } else {
            // If blocked, rotate and find another way
            rotate_right();
        }

        // Print car's current position after each move
        printf("Current Location: (%d, %d)\n", car->current_location.x, car->current_location.y);
    }

    printf("Arrived at destination (%d, %d)!\n", target_location.x, target_location.y);
}
