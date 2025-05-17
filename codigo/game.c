#include "game.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define WINNING_SCORE 10

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

void init_game(GameState *game) {
    // Alocação da matriz
    game->field = malloc(SCREEN_HEIGHT * sizeof(char *));
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        game->field[i] = malloc(SCREEN_WIDTH * sizeof(char));
        memset(game->field[i], ' ', SCREEN_WIDTH); // Preenche com espaços
    }

    // Inicialização do jogo
    game->paddle_left = game->paddle_right = SCREEN_HEIGHT / 2;
    game->ball_x = SCREEN_WIDTH / 2;
    game->ball_y = SCREEN_HEIGHT / 2;
    game->ball_dir_x = 1;
    game->ball_dir_y = 1;
    game->score_left = game->score_right = 0;
    game->quit = false;
    game->status = MENU;
    game->winning_player = 0;
    game->score_history = NULL; // Lista vazia
}

void free_resources(GameState *game) {
    if (game->field) {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            free(game->field[i]);
        }
        free(game->field);
    }

    // Libera a lista de placares...
}

void add_score_to_history(GameState *game) {
    // Aloca um novo nó para o placar atual
    ScoreNode *new_node = malloc(sizeof(ScoreNode));
    new_node->score_left = game->score_left;
    new_node->score_right = game->score_right;
    new_node->next = game->score_history;
    game->score_history = new_node; // Insere no início da lista
}

void update_game(GameState *game) {
    if (game->status != PLAYING) return;

    // Limpa TODA a matriz antes de atualizar
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        memset(game->field[y], ' ', SCREEN_WIDTH); // Preenche com espaços
    }

    // Atualiza posição da bola (garanta que está dentro dos limites)
    game->ball_x += game->ball_dir_x;
    game->ball_y += game->ball_dir_y;
    if (game->ball_y >= 0 && game->ball_y < SCREEN_HEIGHT && 
        game->ball_x >= 0 && game->ball_x < SCREEN_WIDTH) {
        game->field[game->ball_y][game->ball_x] = 'O';
    }

    // Atualiza raquetes (sem rastros)
    for (int i = -1; i <= 1; i++) {
        int left_paddle = game->paddle_left + i;
        int right_paddle = game->paddle_right + i;

        if (left_paddle >= 0 && left_paddle < SCREEN_HEIGHT) {
            game->field[left_paddle][0] = '|'; // Raquete esquerda
        }
        if (right_paddle >= 0 && right_paddle < SCREEN_HEIGHT) {
            game->field[right_paddle][SCREEN_WIDTH - 1] = '|'; // Raquete direita
        }
    }

    // Colisões com bordas (evita bola sumir)
    if (game->ball_y <= 0 || game->ball_y >= SCREEN_HEIGHT - 1) {
        game->ball_dir_y *= -1; // Inverte direção Y
    }
    
    if (game->ball_x <= 0 || game->ball_x >= SCREEN_WIDTH - 1) {
    // Reset da bola no centro
    game->ball_x = SCREEN_WIDTH / 2;
    game->ball_y = SCREEN_HEIGHT / 2;
    game->ball_dir_x = (game->ball_x <= 0) ? 1 : -1; // Direção oposta ao jogador que pontuou
    game->ball_dir_y = (rand() % 2) ? 1 : -1; // Direção Y aleatória
}
}

// (As outras funções permanecem iguais, exceto por render())
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

    // Renderiza TUDO a partir da matriz
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (game->field[y][x] != ' ') {
                mvprintw(y, x, "%c", game->field[y][x]);
            }
        }
    }

    // Placar (adicione diretamente na matriz se preferir)
    mvprintw(0, SCREEN_WIDTH/2 - 5, "%d - %d", game->score_left, game->score_right);
}