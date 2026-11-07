#ifndef PLAYER_H
#define PLAYER_H
#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define ITEM_EMPTY 0

typedef struct Player {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x;
    int y;
    int inventory[MAX_ITEMS];
} Player;

Player *player_create(const char *name);
void player_destroy(Player *player);

#endif