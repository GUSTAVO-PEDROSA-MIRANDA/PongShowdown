#include "game.h"
#include <unistd.h> 
#include <stdio.h>

int main() {
    // Inicialização do jogo
    GameState game;
    
    // Primeiro inicializa a tela
    screenInit(1);      // Modo tela cheia
    screenHideCursor();  // Esconde o cursor
    
    // Depois inicializa o jogo (que agora vai renderizar o menu)
    init_game(&game);
    
    // Loop principal do jogo
    while (!game.quit) {
        handle_input(&game);
        update_game(&game);
        render(&game);
        usleep(30000);
    }
    
    // Finalização
    screenDestroy();
    free_resources(&game);
    return 0;
}