#ifndef MAP_H
#define MAP_H
#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 15
#define BASE_GOLD_MULTIPLIER 10
#define WALL '#'
#define FLOOR '.'
#define TABLE 'T'
#define ROCK 'R'
#define TRAP '^'
#define HERO '@'
#define STAIRS_UP 'U'
#define STAIRS_DOWN 'D'
#define TRAPS_PER_MAP 3

typedef struct {
    int width;
    int height;
    int current_floor;
    char **grid;
    int stairs_up_x, stairs_up_y;
    int stairs_down_x, stairs_down_y;
} Map;

Map* generate_map(int floor);
void free_map(Map **m);
void print_map(const Map *m);
int move_hero(Map *m, char command);

#endif