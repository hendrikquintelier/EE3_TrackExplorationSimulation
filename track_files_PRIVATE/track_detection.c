#include "track_detection.h"
#include "../direction.h"
#include "track_navigation.h"

// Function to update ultrasonic sensor readings
void update_ultrasonic_sensors() {
    int x = current_car.current_location.x;
    int y = current_car.current_location.y;

    // Reset all sensor values to true (assume clear path first)
    ultrasonic_sensors[0] = true;  // Forward
    ultrasonic_sensors[1] = true;  // Left
    ultrasonic_sensors[2] = true;  // Right

    // Determine sensor positions based on current direction
    switch (current_car.current_orientation) {
        case NORTH:
            if (grid[y - 1][x] != TRACK && grid[y - 1][x] != START_FINISH) ultrasonic_sensors[0] = false;  // Forward
        if (grid[y][x - 1] != TRACK && grid[y][x - 1] != START_FINISH) ultrasonic_sensors[1] = false;  // Left
        if (grid[y][x + 1] != TRACK && grid[y][x + 1] != START_FINISH) ultrasonic_sensors[2] = false;  // Right
        break;

        case SOUTH:
            if (grid[y + 1][x] != TRACK && grid[y + 1][x] != START_FINISH) ultrasonic_sensors[0] = false;
        if (grid[y][x + 1] != TRACK && grid[y][x + 1] != START_FINISH) ultrasonic_sensors[1] = false;
        if (grid[y][x - 1] != TRACK && grid[y][x - 1] != START_FINISH) ultrasonic_sensors[2] = false;
        break;

        case WEST:
            if (grid[y][x - 1] != TRACK && grid[y][x - 1] != START_FINISH) ultrasonic_sensors[0] = false;
        if (grid[y + 1][x] != TRACK && grid[y + 1][x] != START_FINISH) ultrasonic_sensors[1] = false;
        if (grid[y - 1][x] != TRACK && grid[y - 1][x] != START_FINISH) ultrasonic_sensors[2] = false;
        break;

        case EAST:
            if (grid[y][x + 1] != TRACK && grid[y][x + 1] != START_FINISH) ultrasonic_sensors[0] = false;
        if (grid[y - 1][x] != TRACK && grid[y - 1][x] != START_FINISH) ultrasonic_sensors[1] = false;
        if (grid[y + 1][x] != TRACK && grid[y + 1][x] != START_FINISH) ultrasonic_sensors[2] = false;
        break;
    }
}
