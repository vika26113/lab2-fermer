#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool input_int(const char *message, int *value) {
    char line[INPUT_BUFFER_SIZE];
    char extra_char;
    printf("%s", message);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return false;
    }
    if (sscanf(line, "%d %c", value, &extra_char) != 1) {
        printf("Ошибка: введите целое число.\n");
        return false;
    }
    return true;
}

bool input_string(const char *message, char *buffer, int buffer_size) {
    printf("%s", message);
    if (fgets(buffer, buffer_size, stdin) == NULL) {
        return false;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        printf("Ошибка: строка не должна быть пустой.\n");
        return false;
    }
    return true;
}

void initialize_random(void) {
    srand((unsigned int)time(NULL));
}