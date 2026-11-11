#ifndef PLAYER_H
#define PLAYER_H
#define MAX_NAME_LEN 32
#define MAX_ITEMS 10

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
} Player;

Player* create_player();
void free_player(Player **p);
void print_player_card(const Player *p);

#endif