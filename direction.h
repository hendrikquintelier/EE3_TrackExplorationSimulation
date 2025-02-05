#ifndef DIRECTION_H
#define DIRECTION_H

// Enum for cardinal directions
typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

// Function to get the opposite direction
Direction opposite_direction(Direction dir);

// Function to turn left from the current direction
Direction turn_left(Direction dir);

// Function to turn right from the current direction
Direction turn_right(Direction dir);

// Function to convert direction to its string representation
const char* direction_to_string(Direction dir);

// Function to convert direction to its corresponding symbol
char direction_to_symbol(Direction dir);

#endif // DIRECTION_H
