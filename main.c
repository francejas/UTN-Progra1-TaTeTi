#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define FILAS 3
#define COLUMNAS 3

void inicializarTablero(char tablero[FILAS][COLUMNAS]);
int mostrarMenuPrincipal();
int mostrarMenuJuego();
void seleccionPiezas(char *p1, char *p2);
void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);
int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna);
int hayGanador(char tablero[FILAS][COLUMNAS], char jugador);
int empate(char tablero[FILAS][COLUMNAS]);
int modoDificultad();
void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina);
void modoMedio(char tablero[FILAS][COLUMNAS], char jugador, char maquina);
void modoDificil(char tablero[FILAS][COLUMNAS], char jugador, char maquina);
void turnoJugador(char tablero[FILAS][COLUMNAS], int *fila, int *columna, char actual);
int minimax(char tablero[FILAS][COLUMNAS], int profundidad, int esMaximizador, char jugador, char maquina);
int evaluarEstado(char tablero[FILAS][COLUMNAS], char jugador, char maquina);

int main() {
    srand(time(NULL));
    char tablero[FILAS][COLUMNAS];
    char p1, p2;
    int salir = 0;

    while (!salir) {
        int opcion = mostrarMenuPrincipal();
        system("cls");

        switch(opcion) {
            case 1: // Partida rápida
            {
                int modo = mostrarMenuJuego();
                inicializarTablero(tablero);
                if (modo == 1) {
                    seleccionPiezas(&p1, &p2);
                    jugarPvP(tablero, p1, p2);
                } else {
                    p1 = 'X';
                    p2 = 'O';
                    int dificultad = modoDificultad();
                    if (dificultad == 1) modoFacil(tablero, p1, p2);
                    else if (dificultad == 2) modoMedio(tablero, p1, p2);
                    else modoDificil(tablero, p1, p2);
                }
                break;
            }
            case 2:
                printf("[Iniciar sesion] Funcionalidad por implementar.\n");
                break;
            case 3:
                printf("[Registrarse] Funcionalidad por implementar.\n");
                break;
            case 4:
                salir = 1;
                break;
            default:
                // no debería ocurrir
                break;
        }
        if (!salir) {
            printf("Presione ENTER para volver al menu principal...");
            getchar(); getchar();
            system("cls");
        }
    }

    return 0;
}

int mostrarMenuPrincipal() {
    int opcion = 0;
    int flag = 1;
    while (flag) {
        printf("=== TA-TE-TI ===\n");
        printf("1 - Partida rapida\n");
        printf("2 - Iniciar sesion\n");
        printf("3 - Registrarse\n");
        printf("4 - Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion >=1 && opcion <=4) flag = 0;
        else printf("Opcion invalida. Intente de nuevo.\n");
    }
    return opcion;
}

int mostrarMenuJuego() {
    int opcion = 0;
    int flag = 1;
    while (flag) {
        printf("=== TA-TE-TI ===\n");
        printf("=== MODO DE JUEGO ===\n");
        printf("1 - Jugador vs Jugador\n");
        printf("2 - Jugador vs Maquina\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion == 1 || opcion == 2) flag = 0;
        else printf("Opcion invalida. Intente de nuevo.\n");
    }
    return opcion;
}

void inicializarTablero(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            tablero[i][j] = ' ';
}


int modoDificultad() {
    int dificultad = 0;
    int flag = 1;

    while (flag) {
        printf("Seleccione dificultad:\n");
        printf("1 - Facil\n");
        printf("2 - Medio\n");
        printf("3 - Dificil\n");
        printf("Eleccion: ");

        scanf("%d", &dificultad);
        if (dificultad >= 1 && dificultad <= 3)
            flag = 0;
        else
            printf("Opcion invalida. Intente de nuevo.\n");
    }

    system("cls");
    return dificultad;
}

void seleccionPiezas(char *p1, char *p2) {
    int flag = 1;

    while (flag) {
        printf("Jugador 1, elija su ficha (X o O): ");
        scanf(" %c", p1);
        *p1 = toupper(*p1);
        if (*p1 == 'X' || *p1 == 'O')
            flag = 0;
        else
            printf("Ficha invalida. Intente de nuevo.\n");
    }

    *p2 = (*p1 == 'X') ? 'O' : 'X';
    printf("Jugador 1: %c | Jugador 2: %c\n", *p1, *p2);
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

void turnoJugador(char tablero[FILAS][COLUMNAS], int *fila, int *columna, char actual) {
    int flag = 1;
    printf("Turno del Jugador (%c)\n", actual);
    while (flag) {
        printf("Ingrese fila (0-2): ");
        scanf("%d", fila);
        printf("Ingrese columna (0-2): ");
        scanf("%d", columna);
        if (movimientoInvalido(tablero, *fila, *columna))
            printf("Movimiento invalido. Intente de nuevo.\n");
        else
            flag = 0;
    }
}

int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna) {
    int invalido = 0;
    if (!(fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS && tablero[fila][columna] == ' '))
        invalido = 1;
    return invalido;
}

int hayGanador(char tablero[FILAS][COLUMNAS], char jugador) {
    int ganador = 0;
    for (int i = 0; i < FILAS && !ganador; i++) {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador)
            ganador = 1;
        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador)
            ganador = 1;
    }
    if (!ganador) {
        if (tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador)
            ganador = 1;
        else if (tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador)
            ganador = 1;
    }
    return ganador;
}

int empate(char tablero[FILAS][COLUMNAS]) {
    int lleno = 1;
    for (int i = 0; i < FILAS && lleno; i++)
        for (int j = 0; j < COLUMNAS; j++)
            if (tablero[i][j] == ' ')
                lleno = 0;
    return lleno;
}

void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2) {
    int turno = 0, fila = 0, columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? p1 : p2;
        turnoJugador(tablero, &fila, &columna, actual);
        tablero[fila][columna] = actual;

        int victoria = hayGanador(tablero, actual);
        int empateJuego = empate(tablero);
        if (victoria || empateJuego) {
            mostrarTablero(tablero);
            if (victoria)
                printf("Jugador %d (%c) gana!\n", (turno % 2) + 1, actual);
            else
                printf("Empate!\n");
            juegoTerminado = 1;
        } else {
            turno++;
        }
    }
}

void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int turno = 0, fila = 0, columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador)
            turnoJugador(tablero, &fila, &columna, actual);
        else {
            do {
                fila = rand() % 3;
                columna = rand() % 3;
            } while (movimientoInvalido(tablero, fila, columna));
        }

        tablero[fila][columna] = actual;
        int victoria = hayGanador(tablero, actual);
        int empateJuego = empate(tablero);
        if (victoria || empateJuego) {
            mostrarTablero(tablero);
            if (victoria)
                printf("%c gana!\n", actual);
            else
                printf("Empate!\n");
            juegoTerminado = 1;
        } else {
            turno++;
        }
    }
}

void modoMedio(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int turno = 0, fila = 0, columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador) {
            turnoJugador(tablero, &fila, &columna, actual);
        } else {
            int bloqueado = 0;
            for (int i = 0; i < FILAS && !bloqueado; i++)
                for (int j = 0; j < COLUMNAS && !bloqueado; j++)
                    if (tablero[i][j] == ' ') {
                        tablero[i][j] = jugador;
                        if (hayGanador(tablero, jugador)) {
                            tablero[i][j] = maquina;
                            fila = i;
                            columna = j;
                            bloqueado = 1;
                        } else
                            tablero[i][j] = ' ';
                    }
            if (!bloqueado) {
                do {
                    fila = rand() % 3;
                    columna = rand() % 3;
                } while (movimientoInvalido(tablero, fila, columna));
            }
        }

        tablero[fila][columna] = actual;
        int victoria = hayGanador(tablero, actual);
        int empateJuego = empate(tablero);
        if (victoria || empateJuego) {
            mostrarTablero(tablero);
            if (victoria)
                printf("%c gana!\n", actual);
            else
                printf("Empate!\n");
            juegoTerminado = 1;
        } else {
            turno++;
        }
    }
}

int evaluarEstado(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int puntaje = 0;
    if (hayGanador(tablero, maquina))
        puntaje = 10;
    else if (hayGanador(tablero, jugador))
        puntaje = -10;
    return puntaje;
}

int minimax(char tablero[FILAS][COLUMNAS], int profundidad, int esMaximizador, char jugador, char maquina) {
    int puntaje = evaluarEstado(tablero, jugador, maquina);
    int resultado = 0;
    if (puntaje == 10 || puntaje == -10 || empate(tablero)) {
        resultado = puntaje;
    } else if (esMaximizador) {
        int mejor = -1000;
        for (int i = 0; i < FILAS; i++)
            for (int j = 0; j < COLUMNAS; j++)
                if (tablero[i][j] == ' ') {
                    tablero[i][j] = maquina;
                    int valor = minimax(tablero, profundidad + 1, 0, jugador, maquina);
                    tablero[i][j] = ' ';
                    if (valor > mejor) mejor = valor;
                }
        resultado = mejor;
    } else {
        int peor = 1000;
        for (int i = 0; i < FILAS; i++)
            for (int j = 0; j < COLUMNAS; j++)
                if (tablero[i][j] == ' ') {
                    tablero[i][j] = jugador;
                    int valor = minimax(tablero, profundidad + 1, 1, jugador, maquina);
                    tablero[i][j] = ' ';
                    if (valor < peor) peor = valor;
                }
        resultado = peor;
    }
    return resultado;
}

void modoDificil(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int turno = 0, fila = 0, columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador) {
            turnoJugador(tablero, &fila, &columna, actual);
        } else {
            int mejorValor = -1000;
            for (int i = 0; i < FILAS; i++)
                for (int j = 0; j < COLUMNAS; j++)
                    if (tablero[i][j] == ' ') {
                        tablero[i][j] = maquina;
                        int valor = minimax(tablero, 0, 0, jugador, maquina);
                        tablero[i][j] = ' ';
                        if (valor > mejorValor) {
                            mejorValor = valor;
                            fila = i;
                            columna = j;
                        }
                    }
        }

        tablero[fila][columna] = actual;
        int victoria = hayGanador(tablero, actual);
        int empateJuego = empate(tablero);
        if (victoria || empateJuego) {
            mostrarTablero(tablero);
            if (victoria)
                printf("%c gana!\n", actual);
            else
                printf("Empate!\n");
            juegoTerminado = 1;
        } else {
            turno++;
        }
    }
}
