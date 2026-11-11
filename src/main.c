#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    srand(time(NULL));
    GameState gs;
    init_game(&gs);
    game_loop(&gs);
    return 0;
}