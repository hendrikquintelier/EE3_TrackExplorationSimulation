#include "track_generation.h"


// Define the global grid (now stored in track_generation.c)
char grid[GRID_SIZE][GRID_SIZE];

// Function to initialize the grid
void initialize_grid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY; // Default to empty space
        }
    }
}

// Function to create a **proper closed-loop track** (1-block width)
void create_loop_track() {
    char complex_grid[GRID_SIZE][GRID_SIZE] = {
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'S', '#', '#', '#', '#', '#', '#', '.', '.', '.', '.'},
        {'.', '.', '#', '.', '.', '#', '.', '.', '#', '.', '.', '.', '.'},
        {'.', '.', '#', '.', '.', '#', '#', '#', '#', '.', '.', '.', '.'},
        {'.', '.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'},
        {'.', '.', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '.'},
        {'.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '.', '#', '.'},
        {'.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.'},
        {'.', '.', '.', '.', '.', '#', '.', '.', '.', '#', '.', '#', '.'},
        {'.', '.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '.'},
        {'.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '#', '.', '.'},
        {'.', '.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    };

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = complex_grid[i][j];  // Copy the layout into the main grid
        }
    }
}
