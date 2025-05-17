#include "game.h"
#include <ncurses.h>
#include <stdlib.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

void init_game(GameState *game) {
    game->paddle_left = game->paddle_right = SCREEN_HEIGHT / 2;
    game->ball_x = SCREEN_WIDTH / 2;
    game->ball_y = SCREEN_HEIGHT / 2;
    game->ball_dir_x = 1;
    game->ball_dir_y = 1;
    game->score_left = game->score_right = 0;
    game->quit = false;
}

void handle_input(GameState *game) {
    int ch = getch();
    switch(ch) {
        case 'w': if (game->paddle_left > 1) game->paddle_left--; break;
        case 's': if (game->paddle_left < SCREEN_HEIGHT-2) game->paddle_left++; break;
        case KEY_UP: if (game->paddle_right > 1) game->paddle_right--; break;
        case KEY_DOWN: if (game->paddle_right < SCREEN_HEIGHT-2) game->paddle_right++; break;
        case 'q': game->quit = true; break;
    }
}

void update_game(GameState *game) {
    // Movimento da bola
    game->ball_x += game->ball_dir_x;
    game->ball_y += game->ball_dir_y;

    // Colisão com topo/fundo
    if (game->ball_y <= 0 || game->ball_y >= SCREEN_HEIGHT-1) 
        game->ball_dir_y *= -1;

    // Colisão com raquetes
    if ((game->ball_x == 1 && abs(game->ball_y - game->paddle_left) <= 1) ||
        (game->ball_x == SCREEN_WIDTH-2 && abs(game->ball_y - game->paddle_right) <= 1)) {
        game->ball_dir_x *= -1;
    }

    // Pontuação
    if (game->ball_x <= 0) {
        game->score_right++;
        game->ball_x = SCREEN_WIDTH / 2;
    }
    if (game->ball_x >= SCREEN_WIDTH-1) {
        game->score_left++;
        game->ball_x = SCREEN_WIDTH / 2;
    }
}

void render(GameState *game) {
    clear();

    // Desenha raquetes
    for (int i = -1; i <= 1; i++) {
        mvprintw(game->paddle_left + i, 0, "|");
        mvprintw(game->paddle_right + i, SCREEN_WIDTH-1, "|");
    }

    // Desenha bola
    mvprintw(game->ball_y, game->ball_x, "O");

    // Placar
    mvprintw(0, SCREEN_WIDTH/2 - 5, "%d - %d", 
             game->score_left, game->score_right);

    // Bordas
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        mvprintw(i, SCREEN_WIDTH/2, "|");
    }
}