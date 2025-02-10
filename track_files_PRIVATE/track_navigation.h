#ifndef TRACK_NAVIGATION_H
#define TRACK_NAVIGATION_H

#include "track_generation.h"
#include "track_detection.h"

// Function declarations
void print_grid();
void rotate_left();
void rotate_right();
void move_forward();
void navigate_to_point(Car *car, Location target_location);


#endif // TRACK_NAVIGATION_H
