#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

void init_game(GameState *gs) {
    gs->day = 1;
    gs->hour = 8;
    gs->player = NULL;
    gs->map = NULL;
    load_catalog(gs->catalog, MAX_ITEMS);
}

void time_menu(GameState *gs) {
    int choice = -1;
    while (choice != 0) {
        printf("\n--- УПРАВЛЕНИЕ ВРЕМЕНЕМ ---\n");
        printf("Текущее время: День %d, %02d:00\n", gs->day, gs->hour);
        printf("1. Посмотреть на часы\n");
        printf("2. Пропустить время (Ждать)\n");
        printf("3. Отдых (Лечь спать)\n");
        printf("0. Назад в главное меню\n");
        printf("Выбор: ");
        choice = input_int(0, 3);
        switch (choice) {
            case 1:
                printf("На часах: День %d, %02d:00\n", gs->day, gs->hour);
                break;
            case 2:
                wait_time(gs);
                break;
            case 3:
                sleep_time(gs);
                break;
            case 0:
                break;
        }
    }
}

void wait_time(GameState *gs) {
    printf("Сколько часов работать? ");
    int hours = input_int(1, 24);
    gs->hour += hours;
    while (gs->hour >= 24) { gs->hour -= 24; gs->day++; }
    printf("Время: День %d, %02d:00\n", gs->day, gs->hour);
}

void sleep_time(GameState *gs) {
    if (!gs->player) { printf("Сначала создайте персонажа!\n"); return; }
    gs->day++;
    gs->hour = 8;
    gs->player->hp = gs->player->max_hp;
    gs->player->stamina = gs->player->max_stamina;
    gs->player->active_buff.type = STATUS_NONE;
    gs->player->active_buff.steps_left = 0;
    printf("Вы хорошо выспались.\n");
}

void write_to_diary(GameState *gs) {
    if (!gs->player) { printf("Нет персонажа!\n"); return; }
    FILE *f = fopen("diary.txt", "a");
    if (!f) { printf("Ошибка файла!\n"); return; }
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
    if (!gs->player) { printf("Нет персонажа!\n"); return; }
    FILE *in = fopen("input.txt", "r");
    if (!in) { printf("Ошибка: input.txt не найден!\n"); return; }
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

void start_expedition(GameState *gs) {
    if (gs->player->stamina <= 0) { 
        printf("Нет сил! Идите спать.\n"); 
        return; 
    }
    if (gs->hour >= 2 && gs->hour < 8) { 
        printf("Ночью походы запрещены!\n"); 
        return; 
    }
    gs->map = generate_map(1);
    int floor = 1;
    char action;
    while (1) {
        printf("\n=== Этаж %d ===\n", floor);
        print_map(gs->map);
        printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Время: %02d:00\n", gs->player->hp, gs->player->max_hp, gs->player->stamina, gs->player->max_stamina, gs->player->gold, gs->hour);
        printf("Действие (W/A/S/D - шаг, Q - выход с 'D'): ");
        action = get_char();
        if (action == 'Q') {
            int hero_y = -1, hero_x = -1;
            for (int i = 0; i < gs->map->height; i++) {
                for (int j = 0; j < gs->map->width; j++) {
                    if (gs->map->grid[i][j] == HERO) {
                        hero_y = i; hero_x = j;
                    }
                }
            }
            if (hero_y == gs->map->stairs_down_y && hero_x == gs->map->stairs_down_x) {
                printf("Вы успешно вернулись домой.\n");
                free_map(&gs->map);
                break;
                } else {
                printf("Выход только с лестницы D!\n");
                continue;
            }
        }
        int res = move_hero(gs->map, action);
        if (res == 'E') {
            printf("Путь заблокирован.\n");
            continue;
        }
        int stamina_cost = 1;
        if (gs->player->active_buff.type == STATUS_STRAIN) {
            stamina_cost = 2;
            printf("[!] Растяжение: -2 выносливости!\n");
            } else if (gs->player->active_buff.type == STATUS_VIGOR) {
            stamina_cost = 0;
            printf("[+] Бодрость: шаг бесплатный!\n");
        }
        gs->player->stamina -= stamina_cost;
        if (gs->player->stamina < 0) gs->player->stamina = 0;
        if (gs->player->active_buff.type == STATUS_POISON) {
            gs->player->hp -= 2;
            printf("[!] Отравление: -2 HP!\n");
        }
        if (gs->player->active_buff.type != STATUS_NONE) {
            gs->player->active_buff.steps_left--;
            if (gs->player->active_buff.steps_left <= 0) {
                gs->player->active_buff.type = STATUS_NONE;
                printf("[+] Эффект закончился.\n");
            }
        }
        if (gs->player->active_buff.type == STATUS_NONE && rand() % 100 < 5) {
            gs->player->active_buff.type = (rand() % 3) + 1;
            gs->player->active_buff.steps_left = 10;
            printf("\n=== ВНИМАНИЕ ===\n");
            if (gs->player->active_buff.type == STATUS_STRAIN) printf("Вы подвернули ногу! (Растяжение)\n");
            if (gs->player->active_buff.type == STATUS_POISON) printf("Вы съели ягоду! (Отравление)\n");
            if (gs->player->active_buff.type == STATUS_VIGOR) printf("Вы нашли родник! (Бодрость)\n");
            printf("================\n");
        }
        if (res == 'U') {
            floor++;
            printf("\n»> Лестница! Этаж %d «<\n", floor);
            gs->player->stamina -= 5;
            gs->player->gold += BASE_GOLD_MULTIPLIER * floor;
            gs->hour++;
            if (gs->hour >= 24) { gs->hour -= 24; gs->day++; }
            free_map(&gs->map);
            gs->map = generate_map(floor);
        }
        if (gs->player->stamina <= 0 || gs->player->hp <= 0 || (gs->hour >= 2 && gs->hour < 8)) {
            printf("\n!!! Обморок или ночь !!!\nСпасатели забрали половину золота.\n");
            gs->player->gold /= 2;
            gs->player->stamina = 0;
            free_map(&gs->map);
            break;
        }
    }
}

void game_loop(GameState *gs) {
    while (1) {
        printf("\n=== Весёлый фермер ===\n");
        printf("1. Создать/пересоздать персонажа\n");
        printf("2. Карточка персонажа\n");
        printf("3. Управление временем и режим дня\n"); 
        printf("4. Инвентарь\n");
        printf("5. Положить предмет\n");
        printf("6. Выбросить предмет\n");
        printf("7. Очистка от мусора\n");
        printf("8. Поиск по названию\n");
        printf("9. Дневник\n");
        printf("10. Скупщик\n");
        printf("11. Отправиться в приключение\n");
        printf("0. Выход\n");
        int choice = input_int(0, 11); 
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
                break;
            case 2: 
                print_player_card(gs->player, gs->catalog); 
                break;
            case 3: 
                time_menu(gs); 
                break; 
            case 4: 
                display_inventory(gs->player, gs->catalog); 
                break;
            case 5: 
                put_item(gs->player, gs->catalog); 
                break;
            case 6: 
                drop_item(gs->player, gs->catalog); 
                break;
            case 7: 
                clean_garbage(gs->player, gs->catalog); 
                break;
            case 8: 
                search_item_by_name(gs->player, gs->catalog); 
                break;
            case 9: 
                write_to_diary(gs); 
                break;
            case 10: 
                evaluate_inventory_at_merchant(gs); 
                break;
            case 11:
                if (gs->player) start_expedition(gs);
                else printf("Сначала создайте персонажа!\n");
                break;
        }
    }
}   