#include "game.h"
#include "../cli-lib/include/screen.h"
#include "../cli-lib/include/keyboard.h"
#include "../cli-lib/include/timer.h"

int main() {
    // Inicializa sistemas da CLI-LIB
    screenInit(1);
    screenHideCursor();
    keyboardInit();
    timerInit(20);  // ~14 FPS

    GameState game;
    init_game(&game);

    while (!game.quit) {
        handle_input(&game);
        
        if (timerTimeOver()) {  // Atualiza no ritmo do timer
            update_game(&game);
            render(&game);
        }
    }

    // Limpeza
    keyboardDestroy();
    screenDestroy();
    free_resources(&game);
    return 0;
}