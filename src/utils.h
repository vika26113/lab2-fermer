#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#define INPUT_BUFFER_SIZE 128

bool input_int(const char *message, int *value);
bool input_string(const char *message, char *buffer, int buffer_size);
void initialize_random(void);

#endif