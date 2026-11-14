#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utils.h"

void init_game(GameState *gs) {
    gs->day = 1;
    gs->hour = 8;
    gs->player = NULL;
    gs->map = NULL;
    load_catalog(gs->catalog, MAX_ITEMS);
}

void wait_time(GameState *gs) {
    int hours;
    printf("Сколько часов работать? ");
    int hours = input_int(1, 24);
    gs->hour += hours;
    while (gs->hour >= 24) { gs->hour -= 24; gs->day++; }
    printf("Время: День %d, %02d:00\n", gs->day, gs->hour);
}

void sleep_time(GameState *gs) {
    if (!gs->player) { 
        printf("Сначала создайте персонажа!\n"); 
        return; 
    }
    gs->day++;
    gs->hour = 8;
    gs->player->hp = gs->player->max_hp;
    gs->player->stamina = gs->player->max_stamina;
    gs->player->active_buff.type = STATUS_NONE;
    gs->player->active_buff.steps_left = 0;
    printf("Вы хорошо выспались.\n");
}

void write_to_diary(GameState *gs) {
    if (!gs->player) { 
        printf("Нет персонажа!\n"); 
        return; 
    }
    FILE *f = fopen("diary.txt", "a");
    if (!f) { 
        printf("Ошибка файла!\n"); 
        return; 
    }
    fprintf(f, "Фермер %s | День %d, %02d:00\n", gs->player->name, gs->day, gs->hour);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = gs->player->inventory[i];
        fprintf(f, "  Слот %d: %s\n", i, gs->catalog[id].name);
    }
    fprintf(f, "---\n");
    fclose(f);
    printf("Записано в diary.txt\n");
}

void evaluate_inventory_at_merchant(GameState *gs) {
    if (!gs->player) { 
        printf("Нет персонажа!\n"); 
        return; 
    }
    FILE *in = fopen("input.txt", "r");
    if (!in) { 
        printf("Ошибка: input.txt не найден!\n"); 
        return; 
    }
    char names[MAX_PRICES][MAX_NAME_LEN];
    int prices[MAX_PRICES];
    int count = 0;
    while (count < MAX_PRICES && fscanf(in, "%31s %d", names[count], &prices[count]) == 2) count++;
    fclose(in);
    FILE *out = fopen("output.txt", "w");
    if (!out) return;
    int total = 0;
    int max_price = -1;
    char max_item_name[MAX_NAME_LEN] = "Нет предметов";
    printf("\n=== ОЦЕНОЧНАЯ ВЕДОМОСТЬ СКУПЩИКА ===\n");
    fprintf(out, "=== ОЦЕНОЧНАЯ ВЕДОМОСТЬ СКУПЩИКА ===\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = gs->player->inventory[i];
        if (id == ITEM_EMPTY) continue;
        int price = 0;
        for (int p = 0; p < count; p++) {
            if (strcmp(gs->catalog[id].name, names[p]) == 0) {
                price = prices[p];
                break;
            }
        }
        printf("Слот %d: [%d] %s — Цена: %d монет\n", i, id, gs->catalog[id].name, price);
        fprintf(out, "Слот %d: [%d] %s — Цена: %d монет\n", i, id, gs->catalog[id].name, price);
        total += price;
        if (price > max_price) {
            max_price = price;
            strcpy(max_item_name, gs->catalog[id].name);
        }
    }
    printf("------------------------------------\n");
    fprintf(out, "------------------------------------\n");
    if (max_price > 0) {
        printf("Самый дорогой предмет: %s (%d монет)\n", max_item_name, max_price);
        fprintf(out, "Самый дорогой предмет: %s (%d монет)\n", max_item_name, max_price);
    } else {
        printf("Нет ценных предметов.\n");
        fprintf(out, "Нет ценных предметов.\n");
    }
    printf("ИТОГОВАЯ ВЫРУЧКА: %d монет\n", total);
    fprintf(out, "ИТОГОВАЯ ВЫРУЧКА: %d монет\n", total);
    fclose(out);
    gs->player->gold += total;
    for (int i = 0; i < INVENTORY_SIZE; i++) gs->player->inventory[i] = ITEM_EMPTY;
    printf("Все продано! Ведомость в output.txt\n");
}

void game_loop(GameState *gs) { 
    while (1) { 
        printf("\n=== Весёлый фермер ===\n"); 
        printf("1. Создать/пересоздать\n2. Карточка\n3. Инвентарь\n4. Положить предмет\n"); 
        printf("5. Выбросить предмет\n6. Очистка от мусора (Вариант 5)\n7. Поиск по названию\n"); 
        printf("8. Дневник\n9. Скупщик (Вариант 8)\n10. Поспать\n11. Ждать\n12. В поход\n0. Выход\n"); 
        int choice = input_int(0, 12); 
        switch (choice) { 
            case 0: 
                if (gs->player) free_player(&gs->player); 
                if (gs->map) free_map(&gs->map); 
                return;
            case 1: 
                if (gs->player) { 
                    printf("Удалить текущего? (1-Да, 0-Нет): "); 
                    if (input_int(0, 1) == 1) free_player(&gs->player); 
                    else break; 
            } 
                gs->player = create_player();
            case 2: 
                print_player_card(gs->player, gs->catalog);
                break;
            case 3: 
                display_inventory(gs->player, gs->catalog); 
                break;
            case 4: 
                put_item(gs->player, gs->catalog); 
                break;
            case 5: 
                drop_item(gs->player, gs->catalog); 
                break;
            case 6: 
                clean_garbage(gs->player, gs->catalog); 
                break;
            case 7: 
                search_item_by_name(gs->player, gs->catalog); 
                break;
            case 8: 
                write_to_diary(gs); 
                break;
            case 9: 
                evaluate_inventory_at_merchant(gs); 
                break;
            case 10: 
                sleep_time(gs); 
                break;
            case 11: 
                wait_time(gs); 
                break;
            case 12:
                if (gs->player) start_expedition(gs);
                else printf("Сначала создайте персонажа!\n");
                break;
        }
    }
}
            
void start_expedition(GameState *gs) {
    if (gs->player == NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }
    if (gs->player->stamina == 0) {
        printf("У вас нет сил на поход! Идите спать.\n");
        return;
    }
    if (gs->hour >= 2 && gs->hour < 8) {
        printf("Ночью походы запрещены!\n");
        return;
    }

    gs->map = generate_map(10, 10, 1);
    char action;
    int floor = 1;
    while (1) {
        printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n", 
               gs->player->hp, gs->player->max_hp, gs->player->stamina, gs->player->max_stamina, 
               gs->player->gold, floor, gs->hour);
        printf("Действие (W - шаг, Q - выход): ");
        action = get_char();
        if (action == 'Q') {
            printf("Вы успешно вернулись домой.\n");
            free_map(&gs->map);
            break;
        }
        int stamina_cost = 1;
        if (gs->player->active_buff.type == STATUS_STRAIN) {
            stamina_cost = 2;
            printf("[!] Растяжение: тратится 2 единицы выносливости!\n");
        } else if (gs->player->active_buff.type == STATUS_VIGOR) {
            stamina_cost = 0;
            printf("[+] Бодрость: шаг бесплатный!\n");
        }
        gs->player->stamina -= stamina_cost;
        if (gs->player->stamina < 0) gs->player->stamina = 0;
        if (gs->player->active_buff.type == STATUS_POISON) {
            gs->player->hp -= 2;
            printf("[!] Отравление: вы теряете 2 HP!\n");
        }
        if (gs->player->active_buff.type != STATUS_NONE) {
            gs->player->active_buff.steps_left--;
            if (gs->player->active_buff.steps_left <= 0) {
                gs->player->active_buff.type = STATUS_NONE;
                printf("[+] Действие эффекта закончилось.\n");
            }
        }
        if (gs->player->active_buff.type == STATUS_NONE) {
            if (rand() % 100 < 5) {
                gs->player->active_buff.type = (rand() % 3) + 1;
                gs->player->active_buff.steps_left = 10;
                printf("\n=== ВНИМАНИЕ ===\n");
                if (gs->player->active_buff.type == STATUS_STRAIN) printf("Вы подвернули ногу! (Растяжение на 10 шагов)\n");
                if (gs->player->active_buff.type == STATUS_POISON) printf("Вы съели странную ягоду! (Отравление на 10 шагов)\n");
                if (gs->player->active_buff.type == STATUS_VIGOR) printf("Вы нашли родник! (Бодрость на 10 шагов)\n");
                printf("================\n");
            }
        }

        if (rand() % 100 < 20) {
            floor++;
            printf("\n»> Вы нашли лестницу на %d этаж! «<\n", floor);
            gs->player->stamina -= 5;
            gs->player->gold += BASE_GOLD_MULTIPLIER * floor;
            gs->hour++;
            if (gs->hour >= 24) { gs->hour -= 24; gs->day++;}
        }

        if (gs->player->stamina <= 0 || gs->player->hp <= 0 || (gs->hour >= 2 && gs->hour < 8)) {
            printf("\n!!! Вы потеряли сознание или наступила ночь !!!\n");
            printf("Спасатели доставили вас домой и забрали половину золота.\n");
            gs->player->gold /= 2;
            gs->player->stamina = 0;
            free_map(&gs->map);
            break;
        }
    }
}

void game_loop(GameState *gs) {
    int choice = 0;
    while (1) {
        printf("\n=== ГЛАВНОЕ МЕНЮ ===\n");
        printf("1. Создать/пересоздать персонажа\n");
        printf("2. Карточка персонажа\n");
        printf("3. Посмотреть время\n");
        printf("4. Ждать\n");
        printf("5. Лечь спать\n");
        printf("6. Отправиться в приключение\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        choice = input_int(0, 6);
        switch (choice) {
            case 1:
                if (gs->player != NULL) {
                    printf("Персонаж уже существует. Удалить? (1 - Да, 0 - Нет): ");
                    if (input_int(0, 1) == 1) {
                        free_player(&gs->player);
                    } else break;
                }
                gs->player = create_player();
                break;
            case 2:
                print_player_card(gs->player);
                break;
            case 3:
                printf("День %d, %02d:00\n", gs->day, gs->hour);
                break;
            case 4:
                wait_time(gs);
                break;
            case 5:
                sleep_time(gs);
                break;
            case 6:
                start_expedition(gs);
                break;
            case 0:
                if (gs->player != NULL) free_player(&gs->player);
                if (gs->map != NULL) free_map(&gs->map);
                printf("Выход из игры.\n");
                return;
        }
    }
}