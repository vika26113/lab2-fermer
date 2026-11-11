#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utils.h"

void init_game(GameState *gs) {
    gs->day = 1;
    gs->hour = 8;
    gs->player = NULL;
    gs->map = NULL;
}

void wait_time(GameState *gs) {
    int hours;
    printf("Сколько часов ждать? (от 1 до 18): ");
    hours = input_int(1, 18);
    int new_hour = gs->hour + hours;
    if (new_hour >= 2 && new_hour < 8 && gs->hour < 2) { 
        printf("Нельзя ждать ночью, фермер должен спать!\n");
        return;
    }
    gs->hour += hours;
    if (gs->hour >= 24) {
        gs->hour -= 24;
        gs->day++;
    }
    printf("Время: День %d, %02d:00\n", gs->day, gs->hour);
}

void sleep_time(GameState *gs) {
    if (gs->player == NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }
    gs->day++;
    gs->hour = 8;
    gs->player->hp = gs->player->max_hp;
    gs->player->stamina = gs->player->max_stamina;
    gs->player->active_buff.type = STATUS_NONE;
    gs->player->active_buff.steps_left = 0;
    printf("Вы хорошо выспались. Здоровье и выносливость восстановлены.\n");
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