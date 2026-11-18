#ifndef ENTITY_H
#define ENTITY_H
#include "player.h"
#include "map.h"

typedef enum {
    ENT_TREE,
    ENT_ROCK,
    ENT_HEAL,
    ENT_MONSTER
} EntityType;

typedef struct {
    EntityType type;
    char symbol;
    int x, y;
    int hp, max_hp;
    int attack, defense;
    int drop_item_id;
    int is_alive;
} Entity;

Entity* generate_entities(Map *m, int floor_level, int *entity_count, Player *p);
void free_entities(Entity **entities);
int interact_with_entity(Entity *e, Player *p, Item *catalog, char *log_msg);

#endif