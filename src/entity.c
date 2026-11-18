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

int interact_with_entity(Entity *e, Player *p, Item *catalog, char *log_msg) {
    if (e->type == ENT_TREE || e->type == ENT_ROCK) {
        e->hp -= p->attack;
        if (e->hp <= 0) {
            e->is_alive = 0;
            int added = 0;
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                if (p->inventory[i] == ITEM_EMPTY) {
                    p->inventory[i] = e->drop_item_id;
                    added = 1;
                    break;
                }
            }
            if (added) sprintf(log_msg, "Вы добыли: %s!", catalog[e->drop_item_id].name);
            else sprintf(log_msg, "Рюкзак полон! Ресурс потерян.");
        } else {
            sprintf(log_msg, "Вы ударили %s. Прочность: %d/%d", catalog[e->drop_item_id].name, e->hp, e->max_hp);
        }
    } else if (e->type == ENT_HEAL) {
        p->hp += 20; if (p->hp > p->max_hp) p->hp = p->max_hp;
        p->stamina += 15; if (p->stamina > p->max_stamina) p->stamina = p->max_stamina;
        e->is_alive = 0;
        sprintf(log_msg, "Вы восстановили здоровье и выносливость!");
    } else if (e->type == ENT_MONSTER) {
        int dmg_to_mon = p->attack - (e->defense / 2);
        if (dmg_to_mon < 1) dmg_to_mon = 1;
        e->hp -= dmg_to_mon;
        if (e->hp <= 0) {
            e->is_alive = 0;
            int gold_drop = 10 + rand() % 20;
            p->gold += gold_drop;
            p->exp += 20;
            sprintf(log_msg, "Монстр побежден! +%d золота, +20 опыта.", gold_drop);
            if (p->exp >= p->level * 20) {
                p->level++;
                p->exp = 0;
                p->max_hp += 10; p->hp = p->max_hp;
                p->attack += 2; p->defense += 1;
                sprintf(log_msg, "Уровень повышен! Теперь у вас %d уровень.", p->level);
            }
        } else {
            int dmg_to_p = e->attack - (p->defense / 2);
            if (dmg_to_p < 1) dmg_to_p = 1;
            p->hp -= dmg_to_p;
            sprintf(log_msg, "Вы нанесли %d урона. Монстр ударил вас на %d.", dmg_to_mon, dmg_to_p);
        }
    }
    return 1;
}