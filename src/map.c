#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "player.h"

void find_free_cell(Map *m, int *y, int *x) {
    do {
        *y = 1 + rand() % (m->height - 2);
        *x = 1 + rand() % (m->width - 2);
    } while (m->grid[*y][*x] != FLOOR);
}

Map* generate_map(int floor) {
    Map *m = (Map*)malloc(sizeof(Map));
    if (m == NULL) return NULL;
    m->height = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
    m->width = MIN_MAP_SIZE + rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1);
    m->current_floor = floor;
    m->grid = (char**)malloc(m->height * sizeof(char*));
    for (int i = 0; i < m->height; i++) {
        m->grid[i] = (char*)malloc(m->width * sizeof(char));
        for (int j = 0; j < m->width; j++) {
            if (i == 0 || i == m->height - 1 || j == 0 || j == m->width - 1) {
                m->grid[i][j] = WALL;
            } else {
                m->grid[i][j] = FLOOR;
            }
        }
    }
    for (int t = 0; t < TRAPS_PER_MAP; t++) {
        int ty, tx;
        find_free_cell(m, &ty, &tx);
        m->grid[ty][tx] = TRAP;
    }
    find_free_cell(m, &m->stairs_up_y, &m->stairs_up_x);
    m->grid[m->stairs_up_y][m->stairs_up_x] = STAIRS_UP;
    int hy, hx;
    find_free_cell(m, &hy, &hx);
    m->grid[hy][hx] = HERO;
    m->stairs_down_y = hy;
    m->stairs_down_x = hx;
}

void free_map(Map **m) {
    if (*m != NULL) {
        for (int i = 0; i < (*m)->height; i++) free((*m)->grid[i]);
        free((*m)->grid);
        printf("Память структуры по адресу %p освобождена\n", (void*)*m);
        free(*m);
        *m = NULL;
    }
}

void print_map(const Map *m) {
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            printf("%c", m->grid[i][j]);
        }
        printf("\n");
    }
}

int move_hero(Map *m, Player *p, char command) {
    int hero_y = -1, hero_x = -1;
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (m->grid[i][j] == HERO) { 
                hero_y = i; 
                hero_x = j; 
            }
        }
    }
    if (hero_y == -1) return 'E';
    int new_y = hero_y, new_x = hero_x;
    if (command == 'W') new_y--;
    else if (command == 'S') new_y++;
    else if (command == 'A') new_x--;
    else if (command == 'D') new_x++;
    else return 'E';
    if (new_y < 0 || new_y >= m->height || new_x < 0 || new_x >= m->width) return 'E';
    char dest = m->grid[new_y][new_x];
    if (dest == WALL) return 'E';
    if (dest == TABLE || dest == ROCK) {
        int push_y = new_y + (new_y - hero_y);
        int push_x = new_x + (new_x - hero_x);
        if (push_y < 0 || push_y >= m->height || push_x < 0 || push_x >= m->width) return 'E';
        if (m->grid[push_y][push_x] != FLOOR && m->grid[push_y][push_x] != TRAP) return 'E';
        m->grid[push_y][push_x] = dest;
    }
    if (p->y == m->stairs_down_y && p->x == m->stairs_down_x) m->grid[p->y][p->x] = STAIRS_DOWN; 
    else if (p->y == m->stairs_up_y && p->x == m->stairs_up_x) m->grid[p->y][p->x] = STAIRS_UP; 
    else m->grid[p->y][p->x] = FLOOR;  
    p->y = new_y; 
    p->x = new_x; 
    m->grid[p->y][p->x] = HERO; 
    if (dest == TRAP) { 
        int ty, tx; 
        find_free_cell(m, &ty, &tx); 
        m->grid[p->y][p->x] = FLOOR; 
        p->y = ty; 
        p->x = tx; 
        m->grid[p->y][p->x] = HERO; 
        printf("Герой наступил на руну и телепортировался!\n"); 
        return 'T';
    }
    if (dest == STAIRS_UP) return 'U';
    return 'M';
}