#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define WINNING_SCORE 10
#define SCORES_FILE "pong_scores.dat"

/* ================== FUNÇÕES PRINCIPAIS ================== */

void reset_ball(GameState *game) {
    game->ball_x = SCREEN_WIDTH / 2;
    game->ball_y = SCREEN_HEIGHT / 2;
    game->ball_dir_x = (rand() % 2) ? 1 : -1;
    game->ball_dir_y = (rand() % 2) ? 1 : -1;
}

void init_game(GameState *game) {
    srand(time(NULL));
    
    game->field = malloc(SCREEN_HEIGHT * sizeof(char *));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        game->field[i] = malloc(SCREEN_WIDTH * sizeof(char));
        memset(game->field[i], ' ', SCREEN_WIDTH);
    }

    game->paddle_left = game->paddle_right = SCREEN_HEIGHT / 2;
    reset_ball(game);
    game->score_left = game->score_right = 0;
    game->quit = false;
    game->status = MENU;
    game->winning_player = 0;
    game->score_history = NULL;
    
    load_scores(game);

    render(game);
}

void free_resources(GameState *game) {
    if (game->field) {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            free(game->field[i]);
        }
        free(game->field);
    }
    
    ScoreNode *current = game->score_history;
    while (current != NULL) {
        ScoreNode *temp = current;
        current = current->next;
        free(temp);
    }
}

/* ================== FUNÇÕES DE RENDERIZAÇÃO ================== */

void draw_paddles(GameState *game) {
    for (int i = -1; i <= 1; i++) {
        int left = game->paddle_left + i;
        if (left >= 0 && left < SCREEN_HEIGHT) {
            screenGotoxy(0, left);
            putchar('|');
        }
        int right = game->paddle_right + i;
        if (right >= 0 && right < SCREEN_HEIGHT) {
            screenGotoxy(SCREEN_WIDTH-1, right);
            putchar('|');
        }
    }
}

void draw_ball(GameState *game) {
    if (game->ball_x >= 0 && game->ball_x < SCREEN_WIDTH && 
        game->ball_y >= 0 && game->ball_y < SCREEN_HEIGHT) {
        screenGotoxy(game->ball_x, game->ball_y);
        putchar('O');
    }
}

void show_menu(GameState *game) {
    screenClear();
    
    screenGotoxy(SCREEN_WIDTH/2 - 5, SCREEN_HEIGHT/2 - 2);
    printf("PONG GAME");
    
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2);
    printf("Pressione ESPACO para comecar");
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 2);
    printf("Pressione Q para sair");
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 4);
    printf("Pressione R para resetar placares");
    
    ScoreNode *current = game->score_history;
    int count = 0;
    while (current != NULL && count < 3) {
        screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 6 + count);
        printf("Placar %d: %d - %d", count + 1, current->score_left, current->score_right);
        current = current->next;
        count++;
    }
    
    screenUpdate();
}

void show_game_over(GameState *game) {
    screenClear();
    
    const char* winner_msg = game->winning_player == 1 ? 
        "Jogador 1 Vencedor (Esquerda)" : "Jogador 2 Vencedor (Direita)";
    
    screenGotoxy(SCREEN_WIDTH/2 - 8, SCREEN_HEIGHT/2 - 2);
    printf("FIM DE JOGO!");
    screenGotoxy(SCREEN_WIDTH/2 - strlen(winner_msg)/2, SCREEN_HEIGHT/2);
    printf("%s", winner_msg);
    
    char final_score[30];
    sprintf(final_score, "Placar final: %d - %d", game->score_left, game->score_right);
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 2);
    printf("%s", final_score);
    
    screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 4);
    printf("Pressione Q para sair");
    
    screenUpdate();
}

void render(GameState *game) {
    screenClear();
    
    switch(game->status) {
        case MENU:
            show_menu(game);
            break;
            
        case PLAYING:
            draw_paddles(game);
            draw_ball(game);
            
            char score[10];
            sprintf(score, "%d - %d", game->score_left, game->score_right);
            screenGotoxy(SCREEN_WIDTH/2 - 3, 0);
            printf("%s", score);
            break;
            
        case GAME_OVER:
            show_game_over(game);
            break;
    }
    
    screenUpdate();
}

/* ================== FUNÇÕES DE INPUT ================== */

void handle_input(GameState *game) {
    if (!keyhit()) return;
    
    int ch = readch();
    
    if (game->status == MENU) {
        if (ch == ' ') game->status = PLAYING;
        else if (ch == 'q') game->quit = true;
        else if (ch == 'r' || ch == 'R') reset_scores(game);
        return;
    }
    
    if (game->status == GAME_OVER && ch == 'q') {
        game->quit = true;
        return;
    }

    switch(ch) {
        case 'w': if (game->paddle_left > 1) game->paddle_left--; break;
        case 's': if (game->paddle_left < SCREEN_HEIGHT-2) game->paddle_left++; break;
        case 'o': if (game->paddle_right > 1) game->paddle_right--; break;  // Seta ↑
        case 'k': if (game->paddle_right < SCREEN_HEIGHT-2) game->paddle_right++; break;  // Seta ↓
        case 27: game->quit = true; break;  // Tecla ESC
    }
}

/* ================== LÓGICA DO JOGO ================== */

void update_game(GameState *game) {
    if (game->status != PLAYING) return;

    game->ball_x += game->ball_dir_x;
    game->ball_y += game->ball_dir_y;

    if (game->ball_y <= 0 || game->ball_y >= SCREEN_HEIGHT - 1) {
        game->ball_dir_y *= -1;
    }

    if (game->ball_x == 1 && abs(game->ball_y - game->paddle_left) <= 1) {
        game->ball_dir_x = 1;
        game->ball_dir_y = (game->ball_y - game->paddle_left) * 0.5;
    }

    if (game->ball_x == SCREEN_WIDTH - 2 && abs(game->ball_y - game->paddle_right) <= 1) {
        game->ball_dir_x = -1;
        game->ball_dir_y = (game->ball_y - game->paddle_right) * 0.5;
    }

    if (game->ball_x < 0) {
        game->score_right++;
        if (game->score_right >= WINNING_SCORE) {
            game->status = GAME_OVER;
            game->winning_player = 2;
            add_score_to_history(game);
        } else {
            reset_ball(game);
        }
    }
    
    if (game->ball_x >= SCREEN_WIDTH) {
        game->score_left++;
        if (game->score_left >= WINNING_SCORE) {
            game->status = GAME_OVER;
            game->winning_player = 1;
            add_score_to_history(game);
        } else {
            reset_ball(game);
        }
    }
}

/* ================== FUNÇÕES DE PLACAR ================== */

void add_score_to_history(GameState *game) {
    ScoreNode *new_node = malloc(sizeof(ScoreNode));
    if (!new_node) return;

    new_node->score_left = game->score_left;
    new_node->score_right = game->score_right;
    new_node->next = game->score_history;
    game->score_history = new_node;
    
    save_scores(game);
}

void save_scores(GameState *game) {
    FILE *file = fopen(SCORES_FILE, "wb");
    if (!file) return;

    ScoreNode *current = game->score_history;
    while (current != NULL) {
        fwrite(current, sizeof(ScoreNode), 1, file);
        current = current->next;
    }

    fclose(file);
}

void load_scores(GameState *game) {
    FILE *file = fopen(SCORES_FILE, "rb");
    if (!file) return;

    ScoreNode temp;
    while (fread(&temp, sizeof(ScoreNode), 1, file) == 1) {
        ScoreNode *new_node = malloc(sizeof(ScoreNode));
        if (!new_node) break;
        
        new_node->score_left = temp.score_left;
        new_node->score_right = temp.score_right;
        new_node->next = game->score_history;
        game->score_history = new_node;
    }

    fclose(file);
}

void reset_scores(GameState *game) {
    ScoreNode *current = game->score_history;
    while (current != NULL) {
        ScoreNode *temp = current;
        current = current->next;
        free(temp);
    }
    game->score_history = NULL;
    
    remove(SCORES_FILE);
    
    if (game->status == MENU) {
        screenClear();
        screenGotoxy(SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 8);
        printf("Placares resetados com sucesso!");
        screenUpdate();
        usleep(1500000);
    }
}