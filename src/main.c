#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "map.h"
#include "utils.h"

int main(void) {
    initialize_random();

    GameState *game = game_create("Фермер");

    if (game == NULL) {
        printf("Не удалось запустить игру.\n");
        return 1;
    }

    printf("День: %d, время: %02d:00\n", game->day, game->hour);
    printf("Игрок: %s\n", game->player->name);
    printf("Здоровье: %d/%d\n",
           game->player->hp,
           game->player->max_hp);
    printf("Карта:\n");

    map_print(game->map);

    game_destroy(game);

    return 0;
}