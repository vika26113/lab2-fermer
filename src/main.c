#include <stdio.h>
#include "game.h"
#include "utils.h"

int main(void) {
    GameState *game = game_create();
    if (game == NULL) {
        printf("Не удалось создать игровое состояние.\n");
        return 1;
    }
    int choice = -1;
    char item_names[MAX_ITEMS][MAX_NAME_LEN] = {
        "Пусто",
        "Дерево",
        "Камень",
        "Семена",
        "Железо",
        "Уголь",
        "Золото",
        "Вода",
        "Трава",
        "Меч"
    };
    
    while (choice != 0) {
        printf("\n=== МЕНЮ ===\n");
        printf("[0] Выход\n");
        printf("[1] Создать или пересоздать персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Посмотреть время\n");
        printf("[4] Пропустить время\n");
        printf("[5] Лечь спать\n");

        if (!input_int("Выберите пункт: ", &choice)) {
            continue;
        }

        switch (choice) {
            case 0:
                printf("Выход из программы.\n");
                break;
            case 1:
                game_create_or_replace_player(game);
                break;
            case 2:
                game_show_player_card(game, item_names);
                break;
            case 3:
                game_show_time(game);
                break;
            case 4:
                game_skip_time(game);
                break;
            case 5:
                game_sleep(game);
                break;
            default:
                printf("Такого пункта меню нет.\n");
                break;
        }
    }

    game_destroy(game);

    return 0;
}