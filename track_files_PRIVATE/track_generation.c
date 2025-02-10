#include "track_generation.h"

/**
 * @brief Global 2D grid representing the track layout.
 *
 * Each cell stores a character representing track elements:
 * - '#' = Track
 * - 'S' = Start/Finish line
 * - '.' = Empty space
 */
char grid[GRID_SIZE][GRID_SIZE];

/**
 * @brief Initializes the grid by setting all cells to EMPTY.
 *
 * This function is typically called before generating a track
 * to ensure a clean starting state.
 */
void initialize_grid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY; // Default to empty space
        }
    }
}

/**
 * @brief Generates a predefined closed-loop track.
 *
 * This function creates a **one-block-wide** closed-loop track
 * by copying a predefined layout into the global grid.
 *
 * - 'S' marks the **start/finish** line.
 * - '#' represents **track paths**.
 * - '.' represents **empty space**.
 */
void create_loop_track() {
    // Predefined track layout (loop track)
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

    // Copy predefined track layout into the global grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = complex_grid[i][j];
        }
    }
}
