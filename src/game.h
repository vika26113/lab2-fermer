#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "player.h"
#define START_DAY 1
#define START_HOUR 8
#define GAME_MAP_WIDTH 10
#define GAME_MAP_HEIGHT 10

typedef struct {
    int day;
    int hour;
    Player *player;
    Map *map;
} GameState;

GameState *game_create(const char *player_name);
void game_destroy(GameState *game);

#endif