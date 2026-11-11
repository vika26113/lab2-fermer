#ifndef PLAYER_H
#define PLAYER_H
#define MAX_NAME_LEN 32
#define MAX_ITEMS 10

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
    int x, y;
    int inventory[MAX_ITEMS];
    int inventory_count;
    Buff active_buff;
} Player;

Player* create_player();
void free_player(Player **p);
void print_player_card(const Player *p);

#endif