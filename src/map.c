#include "map.h"
#include <stdio.h>
#include <stdlib.h>

Map *map_create(int width, int height) {
    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        return NULL;
    }
    map->cells = malloc(height * sizeof(char *));
    if (map->cells == NULL) {
        free(map);
        return NULL;
    }
    for (int y = 0; y < height; y++) {
        map->cells[y] = malloc(width * sizeof(char));
        if (map->cells[y] == NULL) {
            for (int previous_y = 0; previous_y < y; previous_y++) {
                free(map->cells[previous_y]);
            }
            free(map->cells);
            free(map);
            return NULL;
        }
    }
    map->width = width;
    map->height = height;
    map->completed_levels = 0;
    map->location = 0;
    map_fill(map);
    return map;
}

void map_fill(Map *map) {
    if (map == NULL) {
        return;
    }
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (y == 0 ||
                y == map->height - 1 ||
                x == 0 ||
                x == map->width - 1) {
                map->cells[y][x] = WALL;
            } else {
                map->cells[y][x] = FLOOR;
            }
        }
    }
}

void map_print(const Map *map) {
    if (map == NULL) {
        return;
    }
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            printf("%c", map->cells[y][x]);
        }
        printf("\n");
    }
}

void map_destroy(Map *map) {
    if (map == NULL) {
        return;
    }
    for (int y = 0; y < map->height; y++) {
        free(map->cells[y]);
    }
    free(map->cells);
    printf(
        "Память структуры карты по адресу %p освобождена\n",
        (void *)map
    );
    free(map);
}