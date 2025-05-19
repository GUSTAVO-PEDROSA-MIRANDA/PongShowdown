#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "../cli-lib/include/screen.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameStatus;

typedef struct ScoreNode {
    int score_left, score_right;
    struct ScoreNode *next;
} ScoreNode;

typedef struct {
    int paddle_left, paddle_right;
    int ball_x, ball_y;
    int ball_dir_x, ball_dir_y;
    int score_left, score_right;
    bool quit;
    GameStatus status;
    int winning_player;
    char **field;
    ScoreNode *score_history;
} GameState;

// Funções principais
void init_game(GameState *game);
void handle_input(GameState *game);
void update_game(GameState *game);
void render(GameState *game);
void free_resources(GameState *game);

// Funções de placar
void add_score_to_history(GameState *game);
void save_scores(GameState *game);
void load_scores(GameState *game);
void reset_scores(GameState *game);

#endif