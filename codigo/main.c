#include "game.h"
#include <ncurses.h>
#include <unistd.h>

int main() {
    GameState game;
    init_game(&game);  // Já carrega os placares salvos

    initscr(); cbreak(); noecho();
    keypad(stdscr, TRUE); nodelay(stdscr, TRUE); curs_set(0);

    while (!game.quit) {
        handle_input(&game);
        update_game(&game);
        render(&game);
        refresh();
        usleep(50000);
    }

    endwin();
    free_resources(&game);
    return 0;
}