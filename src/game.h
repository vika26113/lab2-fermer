#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "player.h"
#include <stdbool.h>
#define START_DAY 1
#define START_HOUR 8
#define GAME_MAP_WIDTH 10
#define GAME_MAP_HEIGHT 10
#define FLOOR_GOLD_MULTIPLIER 10

typedef struct {
    int day;
    int hour;
    Player *player;
    Map *map;
} GameState;

GameState *game_create(void);
void game_destroy(GameState *game);
void game_create_or_replace_player(GameState *game);
void game_show_player_card(const GameState *game, char item_names[MAX_ITEMS][MAX_NAME_LEN]);
void game_show_time(const GameState *game);
void game_skip_time(GameState *game);
void game_sleep(GameState *game);
bool game_require_player(const GameState *game);
void game_start_expedition(GameState *game);

#endif