#ifndef TRACK_GENERATION_H
#define TRACK_GENERATION_H

#include "../globals.h"

#define GRID_SIZE  13 // 13x13 grid

// Symbols for navigation
#define EMPTY '.'
#define TRACK '#'
#define START_FINISH 'S'


// Global grid, accessible by all files
extern char grid[GRID_SIZE][GRID_SIZE];

// Function declarations
void initialize_grid();
void create_loop_track();

#endif // TRACK_GENERATION_H
