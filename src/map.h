#ifndef MAP_H
#define MAP_H
#include <stdbool.h>
#include "direction.h"
#include "player.h"
#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30
#define WALL '#'
#define FLOOR '.'
#define TABLE 'T'
#define ROCK 'R'
#define TRAP '^'
#define HERO '@'
#define STAIRS_UP 'U'
#define STAIRS_DOWN 'D'

typedef struct {
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
void map_generate(Map *map);
bool map_move_player(
    Map *map,
    Player *player,
    Direction direction
);

#endif