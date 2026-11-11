#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "utils.h"

Player* create_player() {
    Player *p = (Player*)malloc(sizeof(Player));
    if (p == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    printf("Введите имя персонажа: ");
    input_string(p->name, MAX_NAME_LEN);
    p->max_hp = 100;
    p->hp = p->max_hp;
    p->max_stamina = 50;
    p->stamina = p->max_stamina;
    p->gold = 0;
    p->x = 0;
    p->y = 0;
    p->inventory_count = 0;
    printf("Персонаж %s успешно создан!\n", p->name);
    return p;
}

void free_player(Player **p) {
    if (*p != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void*)*p);
        free(*p);
        *p = NULL;
    }
}

void print_player_card(const Player *p) {
    if (p == NULL) return;
    printf("\n--- КАРТОЧКА ПЕРСОНАЖА ---\n");
    printf("Имя: %s\n", p->name);
    printf("Здоровье: %d/%d\n", p->hp, p->max_hp);
    printf("Выносливость: %d/%d\n", p->stamina, p->max_stamina);
    printf("Золото: %d\n", p->gold);
    printf("--------------------------\n");
}