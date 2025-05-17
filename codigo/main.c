#include "game.h"
#include <ncurses.h>
#include <unistd.h>

int main() {
    GameState game;
    init_game(&game);

    // Configura ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    // Loop do jogo
    while (!game.quit) {
        handle_input(&game);
        update_game(&game);
        render(&game);
        refresh();
        usleep(50000); // 50ms (20 FPS)
    }

    endwin();
    return 0;
}