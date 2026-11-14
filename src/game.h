#ifndef GAME_H
#define GAME_H
#include "player.h"
#include "map.h"

typedef struct {
    int day;
    int hour;
    Player *player;
    Map *map;
    Item catalog[MAX_ITEMS];
} GameState;

void init_game(GameState *gs);
void game_loop(GameState *gs);
void wait_time(GameState *gs);
void sleep_time(GameState *gs);
void write_to_diary(GameState *gs);
void evaluate_inventory_at_merchant(GameState *gs);
void start_expedition(GameState *gs);

#endif