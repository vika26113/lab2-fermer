#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int input_int(int min, int max) {
    int value;
    while (scanf("%d", &value) != 1 || value < min || value > max) {
        printf("Ошибка! Введите число от %d до %d: ", min, max);
        clear_input_buffer();
    }
    clear_input_buffer();
    return value;
}

void input_string(char *str, int max_len) {
    if (fgets(str, max_len, stdin) != NULL) {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        }
    }
}

char get_char() {
    char c;
    scanf("%c", &c);
    clear_input_buffer();
    return toupper(c);
}

void load_catalog(Item *catalog, int max_items) {
    FILE *file = fopen("items.txt", "r");
    if (file == NULL) file = fopen("../items.txt", "r");
    if (file == NULL) {
        const char *defaults[] = {"Пусто", "Дерево", "Камень", "Семена", "Железо", "Уголь", "Золото", "Вода", "Трава", "Меч"};
        for (int i = 0; i < max_items; i++) {
            catalog[i].id = i;
            strcpy(catalog[i].name, defaults[i]);
        }
        return;
    }
    for (int i = 0; i < max_items; i++) {
        catalog[i].id = i;
        if (fgets(catalog[i].name, MAX_NAME_LEN, file) != NULL) {
            catalog[i].name[strcspn(catalog[i].name, "\n")] = '\0';
        } else {
            strcpy(catalog[i].name, "Неизвестно");
        }
    }
    fclose(file);
}