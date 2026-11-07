#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Player *player_create(const char *name) {
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        return NULL;
    }
    if (name != NULL) {
        strncpy(player->name, name, MAX_NAME_LEN - 1);
        player->name[MAX_NAME_LEN - 1] = '\0';
    } else {
        strcpy(player->name, "Фермер");
    }
    player->hp = 100;
    player->max_hp = 100;
    player->stamina = 50;
    player->max_stamina = 50;
    player->gold = 0;
    player->x = 0;
    player->y = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        player->inventory[i] = ITEM_EMPTY;
    }
    return player;
}

void player_destroy(Player *player) {
    if (player == NULL) {
        return;
    }
    printf(
        "Память структуры по адресу %p освобождена\n",
        (void *)player
    );
    free(player);
}