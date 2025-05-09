#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    char tablero[3][3];
    int modoDeJuego = mostrarMenu();

    inicializarTablero(tablero);
    mostrarTablero(tablero);

    return 0;
}

int mostrarMenu()
