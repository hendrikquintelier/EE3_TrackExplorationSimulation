//
// Created by Hendrik Quintelier on 05/02/2025.
//
#include "direction.h"

#include <stddef.h>

#include "algorithm_structs_PUBLIC/MapPoint.h"

// Returns the opposite direction
Direction opposite_direction(Direction dir) {
    return (dir + 2) % 4;
}

// Returns the direction to the left
Direction turn_left(Direction dir) {
    return (dir + 3) % 4;
}

// Returns the direction to the right
Direction turn_right(Direction dir) {
    return (dir + 1) % 4;
}

// Converts the direction enum to a string
const char* direction_to_string(Direction dir) {
    switch (dir) {
        case NORTH: return "North";
        case EAST:  return "East";
        case SOUTH: return "South";
        case WEST:  return "West";
        default:    return "Unknown";
    }
}

// Converts the direction enum to its corresponding symbol
char direction_to_symbol(Direction dir) {
    switch (dir) {
        case NORTH: return '^';
        case EAST:  return '>';
        case SOUTH: return 'v';
        case WEST:  return '<';
        default:    return '?';
    }
}

// Determines the direction from start to end MapPoint
Direction determine_direction(MapPoint *start, MapPoint *end) {
    if (!start || !end) return INVALID_DIRECTION;  // Return an invalid direction if NULL pointers

    int dx = end->location.x - start->location.x;
    int dy = end->location.y - start->location.y;

    if (dx > 0 && dy == 0) return EAST;  // Moving right
    if (dx < 0 && dy == 0) return WEST;  // Moving left
    if (dy > 0 && dx == 0) return SOUTH; // Moving down
    if (dy < 0 && dx == 0) return NORTH; // Moving up

    return INVALID_DIRECTION;  // If not a straight move, return invalid
}
