#ifndef UTILS_H
#define UTILS_H
#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define MAX_PRICES 30

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
} Item;

void clear_input_buffer();
int input_int(int min, int max);
void input_string(char *str, int max_len);
char get_char();
void load_catalog(Item *catalog, int max_items);
#endif