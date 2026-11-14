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
    printf("Введите имя фермера (до 31 символа): ");
    input_string(p->name, MAX_NAME_LEN);
    p->max_hp = 100;
    p->hp = p->max_hp;
    p->max_stamina = 50;
    p->stamina = p->max_stamina;
    p->gold = 0;
    
    for (int i = 0; i < INVENTORY_SIZE; i++) p->inventory[i] = ITEM_EMPTY; 
    p->inventory[0] = 1;
    p->inventory[1] = 2; 
    p->inventory[3] = 3; 
    p->inventory[4] = 4; 
    p->inventory[5] = 5; 
    p->inventory[7] = 6; 
    p->inventory[8] = 8; 
    p->active_buff.type = STATUS_NONE; 
    p->active_buff.steps_left = 0; 
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

void print_player_card(const Player *p, const Item *catalog) {
    if (p == NULL) {
        printf("Сначала создайте персонажа!\n"); 
        return; 
    }
    printf("\n--- КАРТОЧКА ПЕРСОНАЖА ---\n");
    printf("Имя: %s\n", p->name);
    printf("Здоровье: %d/%d | Выносливость: %d/%d | Золото: %d\n", p->hp, p->max_hp, p->stamina, p->max_stamina, p->gold);
    printf("Эффект: ");
    switch(p->active_buff.type) {
        case STATUS_STRAIN: printf("Растяжение (%d шагов)\n", p->active_buff.steps_left); 
        break;
        case STATUS_POISON: printf("Отравление (%d шагов)\n", p->active_buff.steps_left); 
        break;
        case STATUS_VIGOR:  printf("Бодрость (%d шагов)\n", p->active_buff.steps_left); 
        break;
        default: printf("Здоров\n"); 
        break;
    }
    printf("--------------------------\n");
}

void display_inventory(const Player *p, const Item *catalog) {
    if (p == NULL) {
        printf("Сначала создайте персонажа!\n"); 
        return; 
    }
    printf("\n--- СОСТОЯНИЕ ИНВЕНТАРЯ ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = p->inventory[i];
        printf("Слот %d: [%d] (%s)\n", i, id, catalog[id].name);
    }
    printf("---------------------------\n");
}

void put_item(Player *p, const Item *catalog) {
    if (p == NULL) return;
    printf("\nВведите номер слота (0-%d): ", INVENTORY_SIZE - 1);
    int slot = input_int(0, INVENTORY_SIZE - 1);
    printf("Введите ID предмета (1-%d): ", MAX_ITEMS - 1);
    int id = input_int(1, MAX_ITEMS - 1);
    p->inventory[slot] = id;
    printf("В слот %d помещен [%d] (%s).\n", slot, id, catalog[id].name);
}

void drop_item(Player *p, const Item *catalog) {
    if (p == NULL) return;
    printf("\nВведите номер слота (0-%d): ", INVENTORY_SIZE - 1);
    int slot = input_int(0, INVENTORY_SIZE - 1);
    if (p->inventory[slot] == ITEM_EMPTY) {
        printf("Слот %d уже пуст!\n", slot);
    } else {
        int id = p->inventory[slot];
        p->inventory[slot] = ITEM_EMPTY;
        printf("Вы выбросили [%d] (%s).\n", id, catalog[id].name);
    }
}

void clean_garbage(Player *p, const Item *catalog) {
    if (p == NULL) return;
    printf("\nВведите ID предмета для очистки (1-%d): ", MAX_ITEMS - 1);
    int id = input_int(1, MAX_ITEMS - 1);
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] == id) {
            p->inventory[i] = ITEM_EMPTY;
            count++;
        }
    }
    printf("Очищено слотов: %d (предмет %s).\n", count, catalog[id].name);
}

void search_item_by_name(const Player *p, const Item *catalog) {
    if (p == NULL) return;
    char query[MAX_NAME_LEN];
    printf("\nВведите название для поиска: ");
    input_string(query, MAX_NAME_LEN);
    int found = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = p->inventory[i];
        if (id != ITEM_EMPTY && strcmp(catalog[id].name, query) == 0) {
            printf("Найдено! Слот %d: [%d] (%s)\n", i, id, catalog[id].name);
            found++;
        }
    }
    if (found == 0) printf("Предмет '%s' не найден.\n", query);
}