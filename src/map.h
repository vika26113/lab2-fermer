#ifndef MAP_H
#define MAP_H

typedef struct Map {
    char **cells;
    int width;
    int height;
    int completed_floors;
    int location;
} Map;

#endif