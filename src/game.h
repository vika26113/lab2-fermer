#ifndef GAME_H
#define GAME_H
#include "map.h"
#include "player.h"
#define START_DAY 1
#define START_HOUR 8

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

#endif