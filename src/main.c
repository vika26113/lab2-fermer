#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "game.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    srand(time(NULL));
    GameState gs;
    init_game(&gs);
    game_loop(&gs);
    return 0;
}