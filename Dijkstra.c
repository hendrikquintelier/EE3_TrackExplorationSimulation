//
// Created by Hendrik Quintelier on 06/02/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "globals.h"
#include "algorithm_structs_PUBLIC/Path.h"

// Priority queue node structure for Dijkstra
typedef struct PriorityQueueNode {
    MapPoint *mapPoint;
    int cost;
    struct PriorityQueueNode *next;
} PriorityQueueNode;

// Push to the priority queue (sorted by cost)
void push(PriorityQueueNode **head, MapPoint *mapPoint, int cost) {
    PriorityQueueNode *newNode = malloc(sizeof(PriorityQueueNode));
    newNode->mapPoint = mapPoint;
    newNode->cost = cost;
    newNode->next = NULL;

    if (*head == NULL || cost < (*head)->cost) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    PriorityQueueNode *current = *head;
    while (current->next != NULL && current->next->cost <= cost) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// Pop the lowest-cost node from the priority queue
MapPoint *pop(PriorityQueueNode **head) {
    if (*head == NULL) return NULL;
    PriorityQueueNode *temp = *head;
    MapPoint *mapPoint = temp->mapPoint;
    *head = temp->next;
    free(temp);
    return mapPoint;
}

// Dijkstra's Algorithm to find the shortest path to a MapPoint TBD
Path find_shortest_path_to_mappoint_tbd(MapPoint *current_map_point) {
    if (num_map_points_tbd == 0) {
        printf("No unexplored MapPoints remaining.\n");
        return (Path){NULL, NULL};
    }

    // Distance and parent tracking
    int *distances = malloc(num_map_points_all * sizeof(int));
    MapPoint **parents = malloc(num_map_points_all * sizeof(MapPoint *));
    if (!distances || !parents) {
        perror("Failed to allocate memory for distances or parents array");
        free(distances);
        free(parents);
        return (Path){NULL, NULL};
    }

    for (int i = 0; i < num_map_points_all; i++) {
        distances[i] = INT_MAX;
        parents[i] = NULL;
    }

    // Priority queue
    PriorityQueueNode *pq = NULL;
    push(&pq, current_map_point, 0);
    distances[current_map_point->id] = 0;

    MapPoint *closest_tbd = NULL;

    printf("🚀 Starting Dijkstra from MapPoint ID %d at (%d, %d)\n",
           current_map_point->id, current_map_point->location.x, current_map_point->location.y);

    // Dijkstra loop
    while (pq != NULL) {
        MapPoint *current = pop(&pq);

        printf("🔍 Visiting MapPoint ID %d at (%d, %d), Distance: %d\n",
               current->id, current->location.x, current->location.y, distances[current->id]);

        // Check if current MapPoint is unexplored
        for (int i = 0; i < num_map_points_tbd; i++) {
            if (map_points_tbd[i] == current) {
                closest_tbd = current;
                break;
            }
        }
        if (closest_tbd != NULL) break;

        // Expand neighbors
        for (int i = 0; i < current->numberOfPaths; i++) {
            FundamentalPath *path = &current->paths[i];
            if (!path->end) continue; // Skip unknown paths

            int new_cost = distances[current->id] + path->distance;
            if (new_cost < distances[path->end->id]) {
                distances[path->end->id] = new_cost;
                parents[path->end->id] = current;
                push(&pq, path->end, new_cost);

                printf("   ↳ Updating path to MapPoint ID %d at (%d, %d) with cost %d\n",
                       path->end->id, path->end->location.x, path->end->location.y, new_cost);
            }
        }
    }

    // No reachable unexplored MapPoint
    if (closest_tbd == NULL) {
        printf("❌ No reachable unexplored MapPoints found.\n");
        free(distances);
        free(parents);
        return (Path){NULL, NULL};
    }

    // Backtrack to reconstruct the path
    Path *bestPath = malloc(sizeof(Path));
    if (!bestPath) {
        perror("Failed to allocate memory for bestPath");
        free(distances);
        free(parents);
        return (Path){NULL, NULL};
    }

    bestPath->start = current_map_point;
    bestPath->end = closest_tbd;
    bestPath->totalDistance = distances[closest_tbd->id];

    printf("✅ Found shortest path to MapPoint ID %d at (%d, %d) with distance %d\n",
           closest_tbd->id, closest_tbd->location.x, closest_tbd->location.y, bestPath->totalDistance);

    // Count path length
int pathLength = 0;
MapPoint *step = closest_tbd;

printf("🔍 Debug: Counting path length...\n");

while (step != current_map_point) {
    if (!step) {
        printf("❌ ERROR: step is NULL while counting path length!\n");
        exit(EXIT_FAILURE);
    }
    if (!parents[step->id]) {
        printf("❌ ERROR: parents[%d] is NULL!\n", step->id);
        exit(EXIT_FAILURE);
    }
    pathLength++;
    step = parents[step->id];
}

printf("✅ Path length determined: %d\n", pathLength);

// Allocate memory for the path (ensure it's large enough)
bestPath->route = malloc(pathLength * sizeof(FundamentalPath *));
if (!bestPath->route) {
    perror("Failed to allocate memory for path route");
    free(bestPath);
    free(distances);
    free(parents);
    return (Path){NULL, NULL};
}

int pathIndex = pathLength - 1;  // Start filling from the end
step = closest_tbd;

printf("🔄 Debug: Backtracking to construct the path...\n");

while (step != current_map_point) {
    MapPoint *prev = parents[step->id];

    // Validate 'prev' before dereferencing
    if (!prev) {
        printf("❌ ERROR: prev is NULL at step ID %d\n", step->id);
        exit(EXIT_FAILURE);
    }

    printf("   ↳ Processing step: %d (prev ID: %d)\n", step->id, prev->id);

    // Check available paths
    for (int i = 0; i < prev->numberOfPaths; i++) {
        if (!prev->paths[i].end) {
            printf("❌ ERROR: prev->paths[%d].end is NULL for prev ID: %d\n", i, prev->id);
            exit(EXIT_FAILURE);
        }

        if (prev->paths[i].end == step) {
            if (pathIndex < 0) {
                printf("❌ ERROR: pathIndex out of bounds (%d)!\n", pathIndex);
                exit(EXIT_FAILURE);
            }

            bestPath->route[pathIndex] = &prev->paths[i]; // ✅ Correct order
            printf("   ➜ Step %d: From ID %d → ID %d, Distance: %d, Direction: %s\n",
                   pathIndex, prev->id, step->id, prev->paths[i].distance,
                   direction_to_string(prev->paths[i].direction));

            pathIndex--;
            break;
        }
    }

    step = prev;
}

// Final validation
if (pathIndex != -1) {
    printf("❌ ERROR: pathIndex should be -1 after filling, but got %d!\n", pathIndex);
    exit(EXIT_FAILURE);
}

printf("✅ Path reconstruction completed successfully!\n");



    free(distances);
    free(parents);

    return *bestPath;
}
