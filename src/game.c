#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

GameState *game_create(void) {
    GameState *game = malloc(sizeof(GameState));
    if (game == NULL) {
        return NULL;
    }
    game->day = START_DAY;
    game->hour = START_HOUR;
    game->player = NULL;
    game->map = NULL;
    return game;
}

void game_create_or_replace_player(GameState *game) {
    if (game == NULL) {
        return;
    }
    if (game->player != NULL) {
        int answer = 0;
        printf("\nПерсонаж уже существует.\n");
        printf("Удалить старого персонажа и создать нового?\n");
        printf("Введите 1 — да, 0 — нет: ");
        if (!input_int("", &answer)) {
            return;
        }
        if (answer != 1) {
            printf("Персонаж не был изменён.\n");
            return;
        }
        player_destroy(game->player);
        game->player = NULL;
    }
    char player_name[MAX_NAME_LEN];
    if (!input_string(
            "Введите имя нового персонажа: ",
            player_name,
            MAX_NAME_LEN
        )) {
        return;
    }
    game->player = player_create(player_name);
    if (game->player == NULL) {
        printf("Ошибка: не удалось создать персонажа.\n");
        return;
    }
    printf(
        "Персонаж \"%s\" успешно создан.\n",
        game->player->name
    );
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

void game_show_player_card(
    const GameState *game,
    char item_names[MAX_ITEMS][MAX_NAME_LEN]
) {
    if (game == NULL || game->player == NULL) {
        printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n");
        return;
    }
    const Player *player = game->player;
    printf("\n=== КАРТОЧКА ПЕРСОНАЖА ===\n");
    printf("Имя: %s\n", player->name);
    printf("Здоровье: %d/%d\n", player->hp, player->max_hp);
    printf("Выносливость: %d/%d\n",
           player->stamina, player->max_stamina);
    printf("Золото: %d\n", player->gold);
    printf("Координаты: x = %d, y = %d\n", player->x, player->y);
    printf("День: %d, время: %02d:00\n", game->day, game->hour);
    printf("\nИнвентарь:\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        int item_id = player->inventory[i];
        if (item_id >= 0 && item_id < MAX_ITEMS) {
            printf("Слот %d: [%d] %s\n",
                   i, item_id, item_names[item_id]);
        } else {
            printf("Слот %d: неизвестный ID %d\n", i, item_id);
        }
    }
    printf("===========================\n");
}