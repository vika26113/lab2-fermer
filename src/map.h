#ifndef MAP_H
#define MAP_H
#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30
#define WALL '#'
#define FLOOR '.'
#define TREE 'T'
#define ROCK 'R'
#define TRAP '^'
#define HERO '@'
#define STAIRS_UP 'U'
#define STAIRS_DOWN 'D'

typedef struct Map {
    char **cells;
    int width;
    int height;
    int completed_levels;
    int location;
} Map;

Map *map_create(int width, int height);
void map_fill(Map *map);
void map_print(const Map *map);
void map_destroy(Map *map);

#endif