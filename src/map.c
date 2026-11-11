#include <stdio.h>
#include <stdlib.h>
#include "map.h"

Map* generate_map(int width, int height, int floor) {
    Map *m = (Map*)malloc(sizeof(Map));
    if (m == NULL) return NULL;
    m->width = width;
    m->height = height;
    m->current_floor = floor;
    m->grid = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        m->grid[i] = (char*)malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            m->grid[i][j] = '.'; 
        }
    }
    return m;
}

void free_map(Map **m) {
    if (*m != NULL) {
        for (int i = 0; i < (*m)->height; i++) {
            free((*m)->grid[i]);
        }
        free((*m)->grid);
        printf("Память структуры по адресу %p освобождена\n", (void*)*m);
        free(*m);
        *m = NULL;
    }
}