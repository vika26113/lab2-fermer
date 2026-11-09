#include "game.h"
#include "utils.h"
#include "direction.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

bool game_require_player(const GameState *game) {
    if (game == NULL || game->player == NULL) {
        printf("Персонаж ещё не создан. Сначала выберите пункт [1].\n");
        return false;
    }

    return true;
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
    if (!game_require_player(game)) {
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

void game_show_time(const GameState *game) {
    if (game == NULL) {
        printf("Игровое состояние не создано.\n");
        return;
    }
    printf("День %d, время %02d:00\n", game->day, game->hour);
}

void game_skip_time(GameState *game) {
    if (game == NULL) {
        printf("Игровое состояние не создано.\n");
        return;
    }
    int hours_to_skip;
    if (!input_int("На сколько часов пропустить время? ", &hours_to_skip)) {
        return;
    }
    if (hours_to_skip <= 0) {
        printf("Введите положительное количество часов.\n");
        return;
    }
    int hours_until_2am = 26 - game->hour;
    if (hours_to_skip > hours_until_2am) {
        printf("Нельзя пропустить время дальше 02:00. Пора ложиться спать.\n");
        return;
    }
    int total_hours = game->hour + hours_to_skip;
    game->day += total_hours / 24;
    game->hour = total_hours % 24;
    printf("Прошло %d ч. Теперь день %d, время %02d:00.\n",
           hours_to_skip, game->day, game->hour);
}

void game_sleep(GameState *game) {
    if (!game_require_player(game)) {
        return;
    }
    game->day++;
    game->hour = START_HOUR;
    game->player->hp = game->player->max_hp;
    game->player->stamina = game->player->max_stamina;
    printf("Фермер поспал. Наступил день %d, время %02d:00.\n",
           game->day, game->hour);
    printf("Здоровье и выносливость восстановлены.\n");
}

static bool find_free_map_cell(
    const Map *map,
    int *result_x,
    int *result_y
) {
    if (map == NULL || map->cells == NULL) {
        return false;
    }
    for (int y = 1; y < map->height - 1; y++) {
        for (int x = 1; x < map->width - 1; x++) {
            if (map->cells[y][x] == FLOOR) {
                *result_x = x;
                *result_y = y;
                return true;
            }
        }
    }
    return false;
}

void game_start_expedition(GameState *game) {
    if (!game_require_player(game)) {
        return;
    }
    Player *player = game->player;
    if (player->stamina <= 0) {
        printf("Нельзя отправиться в поход: выносливость равна нулю.\n");
        return;
    }
    if (game->hour >= 2 && game->hour < 8) {
        printf("Сейчас ночь. Сначала нужно поспать.\n");
        return;
    }
    if (game->map == NULL) {
        game->map = map_create(GAME_MAP_WIDTH, GAME_MAP_HEIGHT);
        if (game->map == NULL) {
            printf("Не удалось создать карту похода.\n");
            return;
        }
    }
    int floor_number = 1;
    bool expedition_active = true;
    map_generate(game->map);
    int down_x;
    int down_y;
    int up_x;
    int up_y;
    if (!find_free_map_cell(game->map, &down_x, &down_y)) {
        printf("На карте нет свободного места.\n");
        map_destroy(game->map);
        game->map = NULL;
        return;
    }
    game->map->cells[down_y][down_x] = STAIRS_DOWN;
    player->x = down_x;
    player->y = down_y;
    game->map->cells[player->y][player->x] = HERO;
    if (!find_free_map_cell(game->map, &up_x, &up_y)) {
        printf("Не удалось разместить лестницу вверх.\n");
        map_destroy(game->map);
        game->map = NULL;
        return;
    }
    game->map->cells[up_y][up_x] = STAIRS_UP;
    while (expedition_active) {
        char command_text[INPUT_BUFFER_SIZE];
        printf(
            "\nHP: %d/%d | Выносливость: %d/%d | Золото: %d"
            " | Этаж: %d | Время: %02d:00\n",
            player->hp,
            player->max_hp,
            player->stamina,
            player->max_stamina,
            player->gold,
            floor_number,
            game->hour
        );
        map_print(game->map);
        if (!input_string(
                "Ход: W/A/S/D, Q — выход на лестнице D: ",
                command_text,
                INPUT_BUFFER_SIZE
            )) {
            continue;
        }
        char command = (char)tolower((unsigned char)command_text[0]);
        Direction direction = DIR_NONE;
        switch (command) {
            case 'w':
                direction = DIR_UP;
                break;
            case 's':
                direction = DIR_DOWN;
                break;
            case 'a':
                direction = DIR_LEFT;
                break;
            case 'd':
                direction = DIR_RIGHT;
                break;
            case 'q':
                direction = DIR_QUIT;
                break;
            default:
                printf("Неизвестная команда.\n");
                continue;
        }
        if (direction == DIR_QUIT) {
            if (player->x == down_x && player->y == down_y) {
                printf("Вы вернулись домой. Золото сохранено.\n");
                expedition_active = false;
            } else {
                printf("Выйти можно только с лестницы D.\n");
            }
            continue;
        }
        int old_x = player->x;
        int old_y = player->y;
        if (!map_move_player(game->map, player, direction)) {
            printf("Туда пройти нельзя.\n");
            continue;
        }
        if (old_x == down_x && old_y == down_y) {
            game->map->cells[old_y][old_x] = STAIRS_DOWN;
        } else if (old_x == up_x && old_y == up_y) {
            game->map->cells[old_y][old_x] = STAIRS_UP;
        }
        player->stamina--;
        game->hour++;

        if (game->hour >= 24) {
            game->hour -= 24;
            game->day++;
        }
        if (player->stamina <= 0 || game->hour == 2) {
            player->gold /= 2;
            printf("Герой выбился из сил и был эвакуирован домой.\n");
            printf("За спасение списана половина золота.\n");
            player->x = 0;
            player->y = 0;
            expedition_active = false;
            continue;
        }
        if (player->x == up_x && player->y == up_y) {
            player->stamina -= 5;
            game->hour++;
            if (game->hour >= 24) {
                game->hour -= 24;
                game->day++;
            }
            if (player->stamina <= 0 || game->hour == 2) {
                player->gold /= 2;
                printf("Герой выбился из сил при подъёме и был эвакуирован.\n");
                expedition_active = false;
                continue;
            }
            floor_number++;
            player->gold += FLOOR_GOLD_MULTIPLIER * floor_number;
            map_generate(game->map);
            if (!find_free_map_cell(game->map, &down_x, &down_y) ||
                !find_free_map_cell(game->map, &up_x, &up_y)) {
                printf("Не удалось подготовить следующий этаж.\n");
                expedition_active = false;
                continue;
            }
            game->map->cells[down_y][down_x] = STAIRS_DOWN;
            game->map->cells[up_y][up_x] = STAIRS_UP;
            player->x = down_x;
            player->y = down_y;
            game->map->cells[player->y][player->x] = HERO;
            printf("Вы перешли на этаж %d и получили золото.\n", floor_number);
        }
    }
    map_destroy(game->map);
    game->map = NULL;
}