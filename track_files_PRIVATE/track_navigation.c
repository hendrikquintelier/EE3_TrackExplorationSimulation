#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep (smooth screen updates)
#include "track_navigation.h"
#include "../direction.h"

/**
 * @brief Prints the grid with the car's current position and orientation.
 *
 * This function clears the screen and redraws the grid with the car's location,
 * showing its current direction.
 */
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

/**
 * @brief Rotates the car 90 degrees counterclockwise.
 */
void rotate_left() {
    current_car.current_orientation = turn_left(current_car.current_orientation);
}

/**
 * @brief Rotates the car 90 degrees clockwise.
 */
void rotate_right() {
    current_car.current_orientation = turn_right(current_car.current_orientation);
}

/**
 * @brief Moves the car forward if the next position is part of the track.
 *
 * The function calculates the new position based on the car's current orientation
 * and moves it only if the next position is a valid track or the start/finish line.
 */
void move_forward() {
    int new_x = current_car.current_location.x;
    int new_y = current_car.current_location.y;

    // Determine next position based on direction
    switch (current_car.current_orientation) {
        case NORTH:
            if (new_y > 0) new_y -= 1;
            break;
        case SOUTH:
            if (new_y < GRID_SIZE - 1) new_y += 1;
            break;
        case WEST:
            if (new_x > 0) new_x -= 1;
            break;
        case EAST:
            if (new_x < GRID_SIZE - 1) new_x += 1;
            break;
        default:
            fprintf(stderr, "Warning: Invalid car orientation detected. Unable to move forward.\n");
            return;
    }

    // Only move if the next position is part of the track
    if (grid[new_y][new_x] == TRACK || grid[new_y][new_x] == START_FINISH) {
        current_car.current_location.x = new_x;
        current_car.current_location.y = new_y;
    }
}
