#include <stdio.h>
#include <string.h>
#include <ctype.h>
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