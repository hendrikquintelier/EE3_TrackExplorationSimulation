#include "track_detection.h"

#include <stdio.h>

#include "../direction.h"
#include "track_navigation.h"

/**
 * @brief Updates the ultrasonic sensor readings based on the car's current location and orientation.
 *
 * This function checks the grid surrounding the car and updates the `ultrasonic_sensors` array
 * to indicate whether movement is possible in the forward, left, and right directions.
 */
void update_ultrasonic_sensors() {
    int x = current_car.current_location.x;
    int y = current_car.current_location.y;

    // Reset all sensor values to true (assume clear path first)
    ultrasonic_sensors[0] = true;  // Forward
    ultrasonic_sensors[1] = true;  // Left
    ultrasonic_sensors[2] = true;  // Right

    // Determine sensor positions based on the car's current orientation
    switch (current_car.current_orientation) {
        case NORTH:
            if (y > 0 && (grid[y - 1][x] != TRACK && grid[y - 1][x] != START_FINISH))
                ultrasonic_sensors[0] = false;  // Forward
            if (x > 0 && (grid[y][x - 1] != TRACK && grid[y][x - 1] != START_FINISH))
                ultrasonic_sensors[1] = false;  // Left
            if (x < GRID_SIZE - 1 && (grid[y][x + 1] != TRACK && grid[y][x + 1] != START_FINISH))
                ultrasonic_sensors[2] = false;  // Right
            break;

        case SOUTH:
            if (y < GRID_SIZE - 1 && (grid[y + 1][x] != TRACK && grid[y + 1][x] != START_FINISH))
                ultrasonic_sensors[0] = false;
            if (x < GRID_SIZE - 1 && (grid[y][x + 1] != TRACK && grid[y][x + 1] != START_FINISH))
                ultrasonic_sensors[1] = false;
            if (x > 0 && (grid[y][x - 1] != TRACK && grid[y][x - 1] != START_FINISH))
                ultrasonic_sensors[2] = false;
            break;

        case WEST:
            if (x > 0 && (grid[y][x - 1] != TRACK && grid[y][x - 1] != START_FINISH))
                ultrasonic_sensors[0] = false;
            if (y < GRID_SIZE - 1 && (grid[y + 1][x] != TRACK && grid[y + 1][x] != START_FINISH))
                ultrasonic_sensors[1] = false;
            if (y > 0 && (grid[y - 1][x] != TRACK && grid[y - 1][x] != START_FINISH))
                ultrasonic_sensors[2] = false;
            break;

        case EAST:
            if (x < GRID_SIZE - 1 && (grid[y][x + 1] != TRACK && grid[y][x + 1] != START_FINISH))
                ultrasonic_sensors[0] = false;
            if (y > 0 && (grid[y - 1][x] != TRACK && grid[y - 1][x] != START_FINISH))
                ultrasonic_sensors[1] = false;
            if (y < GRID_SIZE - 1 && (grid[y + 1][x] != TRACK && grid[y + 1][x] != START_FINISH))
                ultrasonic_sensors[2] = false;
            break;

        default:
            // Handle unexpected values for current_car.current_orientation
            fprintf(stderr, "Warning: Invalid car orientation detected.\n");
            ultrasonic_sensors[0] = false;
            ultrasonic_sensors[1] = false;
            ultrasonic_sensors[2] = false;
            break;
    }
}
