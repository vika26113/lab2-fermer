#ifndef MAP_H
#define MAP_H
#define BASE_GOLD_MULTIPLIER 10

typedef struct {
    int width;
    int height;
    int current_floor;
    char **grid;
} Map;

Map* generate_map(int width, int height, int floor);
void free_map(Map **m);

#endif