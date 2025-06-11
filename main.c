#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "stats.h"

#define FILAS 3
#define COLUMNAS 3

void inicializarTablero(char tablero[FILAS][COLUMNAS]);
int mostrarMenu();
int pedirOpcion(const char *mensaje, int min, int max);
void seleccionPiezas(char *p1, char *p2);
void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2);
void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);
int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna);
int hayGanador(char tablero[FILAS][COLUMNAS], char jugador);
int empate(char tablero[FILAS][COLUMNAS]);
int verificarFinDeJuego(char tablero[FILAS][COLUMNAS], char jugador);
void pedirMovimiento(char tablero[FILAS][COLUMNAS], int *fila, int *columna);
void realizarTurno(char tablero[FILAS][COLUMNAS], char jugador, int *fila, int *columna, int esMaquina);
void bucleJuego(char tablero[FILAS][COLUMNAS], char jugador1, char jugador2, int maquinaJuega);
int modoDificultad();
void modoMedio();
void modoDificil();

int main() {
    srand(time(NULL));
    char tablero[FILAS][COLUMNAS];
    char p1, p2;
    stEstadisticas stats;

    while (1) {
        int modo = mostrarMenu();
        inicializarTablero(tablero);

        if (modo == 1) {
            seleccionPiezas(&p1, &p2);
            bucleJuego(tablero, p1, p2, 0);
        } else {
            p1 = 'X';
            p2 = 'O';
            int dificultad = modoDificultad();
            if (dificultad == 1) {
                bucleJuego(tablero, p1, p2, 1);
            } else if (dificultad == 2) {
                modoMedio();
            } else {
                modoDificil();
            }
        }

        char respuesta;
        printf("Desea jugar otra partida? (s/n): ");
        scanf(" %c", &respuesta);
        system("cls");
        if (respuesta != 's' && respuesta != 'S'){
             break;
        }
    }

    return 0;
}

void inicializarTablero(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            tablero[i][j] = ' ';
}

int mostrarMenu() {
    return pedirOpcion("=== TA-TE-TI ===\n1 - Jugador vs Jugador\n2 - Jugador vs Maquina\nSeleccione una opcion: ", 1, 2);
}

int modoDificultad() {
    return pedirOpcion("=== Dificultad ===\n1 - Facil\n2 - Medio (Proximamente)\n3 - Dificil (Proximamente)\nSeleccione una opcion: ", 1, 3);
}

int pedirOpcion(const char *mensaje, int min, int max) {
    int opcion;
    do {
        printf("%s", mensaje);
        scanf("%d", &opcion);
        if (opcion < min || opcion > max)
            printf("Opcion invalida. Intente de nuevo.\n");
    } while (opcion < min || opcion > max);
    system("cls");
    return opcion;
}

void seleccionPiezas(char *p1, char *p2) {
    do {
        printf("Jugador 1, elija su ficha (X o O): ");
        scanf(" %c", p1);
        *p1 = toupper(*p1);
        if (*p1 != 'X' && *p1 != 'O')
            printf("Ficha invalida. Intente de nuevo.\n");
    } while (*p1 != 'X' && *p1 != 'O');
    *p2 = (*p1 == 'X') ? 'O' : 'X';
    printf("Jugador 1: %c | Jugador 2: %c\n", *p1, *p2);
}

void pedirMovimiento(char tablero[FILAS][COLUMNAS], int *fila, int *columna) {
    do {
        printf("Ingrese fila (0-2): ");
        scanf("%d", fila);
        printf("Ingrese columna (0-2): ");
        scanf("%d", columna);
        if (movimientoInvalido(tablero, *fila, *columna))
            printf("Movimiento invalido. Intente de nuevo.\n");
    } while (movimientoInvalido(tablero, *fila, *columna));
}

void realizarTurno(char tablero[FILAS][COLUMNAS], char jugador, int *fila, int *columna, int esMaquina) {
    if (esMaquina) {
        printf("Turno de la Maquina (%c)...\n", jugador);
        do {
            *fila = rand() % 3;
            *columna = rand() % 3;
        } while (movimientoInvalido(tablero, *fila, *columna));
    } else {
        printf("Turno del Jugador (%c)\n", jugador);
        pedirMovimiento(tablero, fila, columna);
    }
}

int verificarFinDeJuego(char tablero[FILAS][COLUMNAS], char jugador) {
    if (hayGanador(tablero, jugador)) {
        mostrarTablero(tablero);
        printf("%c gana!\n", jugador);
        return 1;
    }
    if (empate(tablero)) {
        mostrarTablero(tablero);
        printf("Empate!\n");
        return 1;
    }
    return 0;
}

void bucleJuego(char tablero[FILAS][COLUMNAS], char jugador1, char jugador2, int maquinaJuega) {
    int turno = 0, fila, columna;
    char actual;

    while (1) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador1 : jugador2;
        int esMaquina = (maquinaJuega && turno % 2 != 0);
        realizarTurno(tablero, actual, &fila, &columna, esMaquina);
        tablero[fila][columna] = actual;
        if (verificarFinDeJuego(tablero, actual)) break;
        turno++;
    }
}

void mostrarTablero(char tablero[FILAS][COLUMNAS]) {
    printf("\n   0   1   2\n");
    for (int i = 0; i < FILAS; i++) {
        printf("%d ", i);
        for (int j = 0; j < COLUMNAS; j++) {
            printf(" %c ", tablero[i][j]);
            if (j < COLUMNAS - 1) printf("|");
        }
        printf("\n");
        if (i < FILAS - 1) printf("  ---+---+---\n");
    }
    printf("\n");
}

int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna) {
    return !(fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS && tablero[fila][columna] == ' ');
}

int hayGanador(char tablero[FILAS][COLUMNAS], char jugador) {
    for (int i = 0; i < FILAS; i++) {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador) return 1;
        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador) return 1;
    }
    if (tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador) return 1;
    if (tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador) return 1;
    return 0;
}

int empate(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            if (tablero[i][j] == ' ') return 0;
    return 1;
}

void modoMedio() {
    printf("Modo medio seleccionado\n");
}

void modoDificil() {
    printf("Modo dificil seleccionado\n");
}

