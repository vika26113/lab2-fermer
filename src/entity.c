#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"
#include "utils.h"

Entity* generate_entities(int width, int height, int floor_level, int *entity_count, Player *p) {
    int area = width * height;
    *entity_count = area / 8;
    if (*entity_count < 10) *entity_count = 10;
    if (*entity_count > 50) *entity_count = 50;
    Entity *ents = (Entity*)malloc((*entity_count) * sizeof(Entity));
    if (!ents) return NULL;
    for (int i = 0; i < *entity_count; i++) {
        int ex, ey;
        do {
            ex = 1 + rand() % (width - 2);
            ey = 1 + rand() % (height - 2);
        } while (ex == p->x && ey == p->y);
        ents[i].x = ex;
        ents[i].y = ey;
        ents[i].is_alive = 1;
        int r = rand() % 100;
        if (r < 40) {
            ents[i].type = ENT_TREE;
            ents[i].symbol = 'T';
            ents[i].hp = 10 + floor_level * 5;
            ents[i].drop_item_id = 1;
            ents[i].attack = 0; ents[i].defense = 0;
        } else if (r < 70) {
            ents[i].type = ENT_ROCK;
            ents[i].symbol = 'R';
            ents[i].hp = 15 + floor_level * 8;
            ents[i].drop_item_id = 2;
            ents[i].attack = 0; ents[i].defense = 0;
        } else if (r < 80) {
            ents[i].type = ENT_HEAL;
            ents[i].symbol = '+';
            ents[i].hp = 1;
            ents[i].drop_item_id = 0;
            ents[i].attack = 0; ents[i].defense = 0;
        } else {
            ents[i].type = ENT_MONSTER;
            ents[i].symbol = 'M';
            ents[i].hp = 20 + floor_level * 10;
            ents[i].attack = 5 + floor_level * 2;
            ents[i].defense = 2 + floor_level;
            ents[i].drop_item_id = 6;
        }
        ents[i].max_hp = ents[i].hp;
    }
    return ents;
}

void free_entities(Entity **entities) {
    if (*entities != NULL) {
        printf("Память массива сущностей по адресу %p освобождена\n", (void*)*entities);
        free(*entities);
        *entities = NULL;
    }
}