#ifndef PLAYER_H
#define PLAYER_H
#include "utils.h"
#define INVENTORY_SIZE 10
#define ITEM_EMPTY 0

typedef enum {
    STATUS_NONE = 0,
    STATUS_STRAIN,
    STATUS_POISON,
    STATUS_VIGOR
} StatusEffect;

typedef struct {
    StatusEffect type;
    int steps_left;
} Buff;

typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int inventory[INVENTORY_SIZE];
    Buff active_buff;
} Player;

Player* create_player();
void free_player(Player **p);
void print_player_card(const Player *p, const Item *catalog);
void display_inventory(const Player *p, const Item *catalog);
void put_item(Player *p, const Item *catalog);
void drop_item(Player *p, const Item *catalog);
void clean_garbage(Player *p, const Item *catalog);
void search_item_by_name(const Player *p, const Item *catalog);

#endif