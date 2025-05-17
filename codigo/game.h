#include <stdbool.h>
#ifndef GAME_H
#define GAME_H

typedef struct {
    int paddle_left, paddle_right;
    int ball_x, ball_y;
    int ball_dir_x, ball_dir_y;
    int score_left, score_right;
    bool quit;
} GameState;

void init_game(GameState *game);
void handle_input(GameState *game);
void update_game(GameState *game);
void render(GameState *game);

#endif