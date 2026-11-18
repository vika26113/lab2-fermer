#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"
#include "entity.h"

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
    if (gs->hour >= 2 && gs->hour < 8) {
        printf("Сейчас глубокая ночь/раннее утро. Фермер должен спать! Используйте 'Лечь спать'.\n");
        return;
    }
    printf("Сколько часов работать/ждать? ");
    int hours = input_int(1, 24);
    int total_hours = gs->hour + hours;
    int target_hour = total_hours % 24;
    int target_day = gs->day + (total_hours / 24);
    if (target_hour >= 2 && target_hour < 8) {
        if (target_hour != 2) {
            printf("Нельзя! Вы пересечете границу 02:00 ночи, а фермер должен спать.\n");
            printf("Максимальное время ожидания сейчас: %d ч.\n", (26 - gs->hour) % 24);
            return;
        }
    }
    gs->hour = target_hour;
    gs->day = target_day;
    printf("Время пролетело! Теперь: День %d, %02d:00\n", gs->day, gs->hour);
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
    if (gs->player->stamina <= 0) { printf("Нет сил! Идите спать.\n"); return; }
    if (gs->hour >= 2 && gs->hour < 8) { printf("Ночью походы запрещены!\n"); return; }
    gs->map = generate_map(1);
    int entity_count = 0;
    Entity *entities = generate_entities(gs->map->width, gs->map->height, 1, &entity_count, gs->player);
    for (int i = 0; i < gs->map->height; i++) {
        for (int j = 0; j < gs->map->width; j++) {
            if (gs->map->grid[i][j] == HERO) { 
                gs->player->x = j; 
                gs->player->y = i; 
            }
        }
    }
    char log_msg[128] = "Вы вошли в башню. Будьте осторожны!";
    int floor = 1;
    while (1) {
        system("cls"); 
        for (int i = 0; i < gs->map->height; i++) {
            for (int j = 0; j < gs->map->width; j++) {
                char c = gs->map->grid[i][j];
                if (gs->player->y == i && gs->player->x == j) c = HERO;
                else {
                    for (int k = 0; k < entity_count; k++) {
                        if (entities[k].is_alive && entities[k].y == i && entities[k].x == j) {
                            c = entities[k].symbol; break;
                        }
                    }
                }
                printf("%c", c);
            }
            printf("\n");
        }
        printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n", 
               gs->player->hp, gs->player->max_hp, gs->player->stamina, gs->player->max_stamina, 
               gs->player->gold, floor, gs->hour);
        printf("Последнее событие: %s\n", log_msg);
        printf("Действие (W/A/S/D - шаг, Q - выход с 'D'): ");
        char action = get_char();
        if (action == 'Q') {
            if (gs->player->y == gs->map->stairs_down_y && gs->player->x == gs->map->stairs_down_x) {
                printf("Вы успешно вернулись домой.\n");
                break;
            } else {
                strcpy(log_msg, "Выход только с лестницы D!");
                continue;
            }
        }
        int new_y = gs->player->y;
        int new_x = gs->player->x;
        if (action == 'W') new_y--;
        else if (action == 'S') new_y++;
        else if (action == 'A') new_x--;
        else if (action == 'D') new_x++;
        else { 
            strcpy(log_msg, "Неизвестная команда."); 
            continue; 
        }
        Entity *target = NULL;
        for (int i = 0; i < entity_count; i++) {
            if (entities[i].is_alive && entities[i].x == new_x && entities[i].y == new_y) {
                target = &entities[i]; break;
            }
        }
        if (target) {
            gs->player->stamina -= 2;
            if (gs->player->stamina < 0) gs->player->stamina = 0;
            interact_with_entity(target, gs->player, gs->catalog, log_msg);
        } else {
            int res = move_hero(gs->map, gs->player, action);
            if (res == 'E') {
                strcpy(log_msg, "Путь заблокирован.");
                continue;
            }
            int stamina_cost = 1;
            if (gs->player->active_buff.type == STATUS_STRAIN) stamina_cost = 2;
            else if (gs->player->active_buff.type == STATUS_VIGOR) stamina_cost = 0;
            gs->player->stamina -= stamina_cost;
            if (gs->player->stamina < 0) gs->player->stamina = 0;
            if (gs->player->active_buff.type == STATUS_POISON) gs->player->hp -= 2;
            if (gs->player->active_buff.type != STATUS_NONE) {
                gs->player->active_buff.steps_left--;
                if (gs->player->active_buff.steps_left <= 0) gs->player->active_buff.type = STATUS_NONE;
            }
            if (gs->player->active_buff.type == STATUS_NONE && rand() % 100 < 5) {
                gs->player->active_buff.type = (rand() % 3) + 1;
                gs->player->active_buff.steps_left = 10;
                strcpy(log_msg, "Вы получили случайный эффект!");
            }
            if (res == 'T') strcpy(log_msg, "Вы наступили на руну и телепортировались!");
            if (res == 'U') {
                floor++;
                gs->player->stamina -= 5;
                gs->player->gold += BASE_GOLD_MULTIPLIER * floor;
                gs->hour++;
                if (gs->hour >= 24) { gs->hour -= 24; gs->day++; }
                free_map(&gs->map);
                free_entities(&entities);
                gs->map = generate_map(floor);
                entities = generate_entities(gs->map->width, gs->map->height, floor, &entity_count, gs->player);
                for (int i = 0; i < gs->map->height; i++) {
                    for (int j = 0; j < gs->map->width; j++) {
                        if (gs->map->grid[i][j] == HERO) { gs->player->x = j; gs->player->y = i; }
                    }
                }
                strcpy(log_msg, "Вы поднялись на новый этаж!");
            }
        }
        if (gs->player->stamina <= 0 || gs->player->hp <= 0 || (gs->hour >= 2 && gs->hour < 8)) {
            printf("\n!!! Обморок или ночь !!!\nСпасатели забрали половину золота.\n");
            gs->player->gold /= 2;
            gs->player->stamina = 0;
            break;
        }
    }
    FILE *f = fopen("diary.txt", "a");
    if (f) {
        fprintf(f, "=== ОТЧЕТ О ПОХОДЕ ===\n");
        fprintf(f, "Достигнут этаж: %d\n", floor);
        fprintf(f, "Золото: %d\n", gs->player->gold);
        fprintf(f, "======================\n\n");
        fclose(f);
    }
    free_map(&gs->map);
    free_entities(&entities);
}

void game_loop(GameState *gs) {
    while (1) {
        printf("\n=== Весёлый фермер ===\n");
        printf("[1] Создать/пересоздать персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Управление временем и режим дня\n"); 
        printf("[4] Инвентарь\n");
        printf("[5] Положить предмет\n");
        printf("[6] Выбросить предмет\n");
        printf("[7] Очистка от мусора\n");
        printf("[8] Поиск по названию\n");
        printf("[9] Дневник\n");
        printf("[10] Скупщик\n");
        printf("[11] Отправиться в приключение\n");
        printf("[0] Выход\n");
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
                print_player_card(gs->player, gs->catalog, gs->day, gs->hour); 
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