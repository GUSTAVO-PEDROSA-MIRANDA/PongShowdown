#include "game.h"
#include <unistd.h>
#include <stdio.h>

int main() {
    GameState game;
    
    // Debug 1: Verifique se a inicialização ocorre
    printf("Iniciando jogo...\n");
    init_game(&game);
    
    // Debug 2: Verifique a inicialização da tela
    printf("Inicializando tela...\n");
    screenInit(1);  // A função screenInit original não retorna valor
    screenHideCursor();
    printf("Tela inicializada com sucesso!\n");
    
    // Debug 3: Verifique o tamanho do terminal
    printf("Tamanho do terminal: %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Loop principal do jogo
    while (!game.quit) {
        // Debug 4: Verifique estado atual
        printf("Estado atual: %d | ", game.status);
        
        handle_input(&game);
        update_game(&game);
        render(&game);
        
        // Debug 5: Verifique FPS
        printf("Frame concluído\n");
        usleep(50000);  // ~20 FPS
    }
    
    // Finalização
    printf("Encerrando jogo...\n");
    screenDestroy();
    free_resources(&game);
    printf("Jogo encerrado com sucesso!\n");
    return 0;
}