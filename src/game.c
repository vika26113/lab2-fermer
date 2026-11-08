#include "game.h"

#include <stdio.h>
#include <stdlib.h>

GameState *game_create(const char *player_name) {
    GameState *game = malloc(sizeof(GameState));
    if (game == NULL) {
        return NULL;
    }
    game->day = START_DAY;
    game->hour = START_HOUR;
    game->player = NULL;
    game->map = NULL;
    game->player = player_create(player_name);
    if (game->player == NULL) {
        printf("Не удалось создать игрока.\n");
        free(game);
        return NULL;
    }
    game->map = map_create(GAME_MAP_WIDTH, GAME_MAP_HEIGHT);
    if (game->map == NULL) {
        printf("Не удалось создать карту.\n");
        player_destroy(game->player);
        free(game);
        return NULL;
    }
    game->player->x = 1;
    game->player->y = 1;
    game->map->cells[game->player->y][game->player->x] = HERO;
    return game;
}

void game_destroy(GameState *game) {
    if (game == NULL) {
        return;
    }
    map_destroy(game->map);
    player_destroy(game->player);
    printf(
        "Память структуры GameState по адресу %p освобождена\n",
        (void *)game
    );
    free(game);
}