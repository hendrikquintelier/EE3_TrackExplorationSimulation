#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Windows key input
#include <unistd.h> // For usleep (smooth screen updates)
#include "track_navigation.h"

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
                printf("%c ", current_car.current_orientation);  // Show car's direction
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
    switch (current_car.current_orientation) {
        case LOOK_UP: current_car.current_orientation = LOOK_LEFT; break;
        case LOOK_DOWN: current_car.current_orientation = LOOK_RIGHT; break;
        case LOOK_LEFT: current_car.current_orientation = LOOK_DOWN; break;
        case LOOK_RIGHT: current_car.current_orientation = LOOK_UP; break;
    }
}

// Function to rotate the car right
void rotate_right() {
    switch (current_car.current_orientation) {
        case LOOK_UP: current_car.current_orientation = LOOK_RIGHT; break;
        case LOOK_DOWN: current_car.current_orientation = LOOK_LEFT; break;
        case LOOK_LEFT: current_car.current_orientation = LOOK_UP; break;
        case LOOK_RIGHT: current_car.current_orientation = LOOK_DOWN; break;
    }
}

// Function to move the car forward if possible
void move_forward() {
    int new_x = current_car.current_location.x;
    int new_y = current_car.current_location.y;

    // Determine next position based on direction
    switch (current_car.current_orientation) {
        case LOOK_UP: new_y -= 1; break;
        case LOOK_DOWN: new_y += 1; break;
        case LOOK_LEFT: new_x -= 1; break;
        case LOOK_RIGHT: new_x += 1; break;
    }

    // Only move if the next position is part of the track
    if (grid[new_y][new_x] == TRACK || grid[new_y][new_x] == START_FINISH) {
        current_car.current_location.x = new_x;
        current_car.current_location.y = new_y;
    }
}
