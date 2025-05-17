#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameStatus;

typedef struct {
    int paddle_left, paddle_right;
    int ball_x, ball_y;
    int ball_dir_x, ball_dir_y;
    int score_left, score_right;
    bool quit;
    GameStatus status;
    int winning_player; // 1 para esquerda, 2 para direita
} GameState;

void init_game(GameState *game);
void handle_input(GameState *game);
void update_game(GameState *game);
void render(GameState *game);
void show_menu(GameState *game);
void show_game_over(GameState *game);

#endif