#include "game.h"
#include <ncurses.h>
#include <stdlib.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define WINNING_SCORE 10

void init_game(GameState *game) {
    game->paddle_left = game->paddle_right = SCREEN_HEIGHT / 2;
    game->ball_x = SCREEN_WIDTH / 2;
    game->ball_y = SCREEN_HEIGHT / 2;
    game->ball_dir_x = 1;
    game->ball_dir_y = 1;
    game->score_left = game->score_right = 0;
    game->quit = false;
    game->status = MENU;
    game->winning_player = 0;
}

void show_menu(GameState *game) {
    clear();
    mvprintw(SCREEN_HEIGHT/2 - 2, SCREEN_WIDTH/2 - 10, "PONG GAME");
    mvprintw(SCREEN_HEIGHT/2, SCREEN_WIDTH/2 - 15, "Pressione ESPACO para comecar");
    mvprintw(SCREEN_HEIGHT/2 + 2, SCREEN_WIDTH/2 - 15, "Pressione Q para sair");
    refresh();
}

void show_game_over(GameState *game) {
    clear();
    const char* winner_msg = game->winning_player == 1 ? 
        "Jogador 1 Vencedor (Esquerda)" : "Jogador 2 Vencedor (Direita)";
    
    mvprintw(SCREEN_HEIGHT/2 - 2, SCREEN_WIDTH/2 - 15, "FIM DE JOGO!");
    mvprintw(SCREEN_HEIGHT/2, SCREEN_WIDTH/2 - strlen(winner_msg)/2, "%s", winner_msg);
    mvprintw(SCREEN_HEIGHT/2 + 2, SCREEN_WIDTH/2 - 15, "Placar final: %d - %d", 
             game->score_left, game->score_right);
    mvprintw(SCREEN_HEIGHT/2 + 4, SCREEN_WIDTH/2 - 15, "Pressione Q para sair");
    refresh();
}

void handle_input(GameState *game) {
    int ch = getch();
    
    if (game->status == MENU) {
        if (ch == ' ') {
            game->status = PLAYING;
        } else if (ch == 'q') {
            game->quit = true;
        }
        return;
    }
    
    if (game->status == GAME_OVER) {
        if (ch == 'q') {
            game->quit = true;
        }
        return;
    }

    switch(ch) {
        case 'w': if (game->paddle_left > 1) game->paddle_left--; break;
        case 's': if (game->paddle_left < SCREEN_HEIGHT-2) game->paddle_left++; break;
        case KEY_UP: if (game->paddle_right > 1) game->paddle_right--; break;
        case KEY_DOWN: if (game->paddle_right < SCREEN_HEIGHT-2) game->paddle_right++; break;
        case 'q': game->quit = true; break;
    }
}

void update_game(GameState *game) {
    if (game->status != PLAYING) return;

    game->ball_x += game->ball_dir_x;
    game->ball_y += game->ball_dir_y;

    if (game->ball_y <= 0 || game->ball_y >= SCREEN_HEIGHT-1) 
        game->ball_dir_y *= -1;

    if ((game->ball_x == 1 && abs(game->ball_y - game->paddle_left) <= 1) ||
        (game->ball_x == SCREEN_WIDTH-2 && abs(game->ball_y - game->paddle_right) <= 1)) {
        game->ball_dir_x *= -1;
    }

    if (game->ball_x <= 0) {
        game->score_right++;
        game->ball_x = SCREEN_WIDTH / 2;
    }
    if (game->ball_x >= SCREEN_WIDTH-1) {
        game->score_left++;
        game->ball_x = SCREEN_WIDTH / 2;
    }

    // Verifica se alguém ganhou
    if (game->score_left >= WINNING_SCORE || game->score_right >= WINNING_SCORE) {
        game->status = GAME_OVER;
        game->winning_player = game->score_left >= WINNING_SCORE ? 1 : 2;
    }
}

void render(GameState *game) {
    clear();

    if (game->status == MENU) {
        show_menu(game);
        return;
    }

    if (game->status == GAME_OVER) {
        show_game_over(game);
        return;
    }

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

    // Linha central
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        mvprintw(i, SCREEN_WIDTH/2, "|");
    }
}