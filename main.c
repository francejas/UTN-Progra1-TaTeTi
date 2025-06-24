#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define FILAS 3
#define COLUMNAS 3

#define AR_JUGADORES "jugadores.dat"
#define AR_PARTIDAS "partidas.dat"
#define AR_PARTIDAXJUGADOR "partidaXjugador.dat"

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
int verificarEstadoJuego(char tablero[FILAS][COLUMNAS], char actual, int *juegoTerminado, int *turno, int esJugador);
int preguntarOtraPartida();

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
                int jugarOtra = 1;
                int modo = mostrarMenuJuego();

                while (jugarOtra) {
                    inicializarTablero(tablero);

                    if (modo == 1) {
                        if (jugarOtra == 1) { // Solo preguntar fichas la primera vez
                            seleccionPiezas(&p1, &p2);
                        }
                        jugarPvP(tablero, p1, p2);
                    } else {
                        p1 = 'X';
                        p2 = 'O';
                        int dificultad = 0;
                        if (jugarOtra == 1) { // Solo preguntar dificultad la primera vez
                            dificultad = modoDificultad();
                        }
                        static int dificultadGuardada = 0; // Guardar la dificultad elegida
                        if (jugarOtra == 1) {
                            dificultadGuardada = dificultad;
                        }

                        if (dificultadGuardada == 1) {
                            modoFacil(tablero, p1, p2);
                        } else if (dificultadGuardada == 2) {
                            modoMedio(tablero, p1, p2);
                        } else {
                            modoDificil(tablero, p1, p2);
                        }
                    }

                    jugarOtra = preguntarOtraPartida();
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
    int resultado = 0;

    while (flag) {
        printf("=== TA-TE-TI ===\n");
        printf("1 - Partida rapida\n");
        printf("2 - Iniciar sesion\n");
        printf("3 - Registrarse\n");
        printf("4 - Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion >= 1 && opcion <= 4) {
            flag = 0;
            resultado = opcion;
        } else {
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }

    return resultado;
}

int preguntarOtraPartida() {
    char respuesta;
    int flag = 1;
    int resultado = 0;

    while (flag) {
        printf("\n¿Desea jugar otra partida? (s/n): ");
        scanf(" %c", &respuesta);
        respuesta = tolower(respuesta);
        if (respuesta == 's') {
            resultado = 1;
            flag = 0;
        } else if (respuesta == 'n') {
            resultado = 0;
            flag = 0;
        } else {
            printf("Opcion invalida. Ingrese 's' para si o 'n' para no.\n");
        }
    }

    if (resultado == 1) {
        system("cls");
    }

    return resultado;
}

int mostrarMenuJuego() {
    int opcion = 0;
    int flag = 1;
    int resultado = 0;

    while (flag) {
        printf("=== TA-TE-TI ===\n");
        printf("=== MODO DE JUEGO ===\n");
        printf("1 - Jugador vs Jugador\n");
        printf("2 - Jugador vs Maquina\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion == 1 || opcion == 2) {
            flag = 0;
            resultado = opcion;
        } else {
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }

    return resultado;
}

void inicializarTablero(char tablero[FILAS][COLUMNAS]) {
    int i, j;
    for (i = 0; i < FILAS; i++) {
        for (j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = ' ';
        }
    }
}

int modoDificultad() {
    int dificultad = 0;
    int flag = 1;
    int resultado = 0;

    while (flag) {
        printf("Seleccione dificultad:\n");
        printf("1 - Facil\n");
        printf("2 - Medio\n");
        printf("3 - Dificil\n");
        printf("Eleccion: ");

        scanf("%d", &dificultad);
        if (dificultad >= 1 && dificultad <= 3) {
            flag = 0;
            resultado = dificultad;
        } else {
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }

    system("cls");
    return resultado;
}

void seleccionPiezas(char *p1, char *p2) {
    int flag = 1;
    char entrada;

    while (flag) {
        printf("Jugador 1, elija su ficha (X o O): ");
        scanf(" %c", &entrada);
        *p1 = toupper(entrada);
        if (*p1 == 'X' || *p1 == 'O') {
            flag = 0;
        } else {
            printf("Ficha invalida. Intente de nuevo.\n");
        }
    }

    *p2 = (*p1 == 'X') ? 'O' : 'X';
    printf("Jugador 1: %c | Jugador 2: %c\n", *p1, *p2);
}

void mostrarTablero(char tablero[FILAS][COLUMNAS]) {
    int i, j;
    printf("\n   0   1   2\n");

    for (i = 0; i < FILAS; i++) {
        printf("%d ", i);
        for (j = 0; j < COLUMNAS; j++) {
            printf(" %c ", tablero[i][j]);
            if (j < COLUMNAS - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (i < FILAS - 1) {
            printf("  ---+---+---\n");
        }
    }
    printf("\n");
}

void turnoJugador(char tablero[FILAS][COLUMNAS], int *fila, int *columna, char actual) {
    int flag = 1;
    int filaTemp, columnaTemp;

    printf("Turno del Jugador (%c)\n", actual);

    while (flag) {
        printf("Ingrese fila (0-2): ");
        scanf("%d", &filaTemp);
        printf("Ingrese columna (0-2): ");
        scanf("%d", &columnaTemp);
        if (!movimientoInvalido(tablero, filaTemp, columnaTemp)) {
            *fila = filaTemp;
            *columna = columnaTemp;
            flag = 0;
        } else {
            printf("Movimiento invalido. Intente de nuevo.\n");
        }
    }
}

int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna) {
    int invalido = 1;

    if (fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS && tablero[fila][columna] == ' ') {
        invalido = 0;
    }

    return invalido;
}

int hayGanador(char tablero[FILAS][COLUMNAS], char jugador) {
    int ganador = 0;
    int i;

    // Verificar filas y columnas
    for (i = 0; i < FILAS && !ganador; i++) {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador) {
            ganador = 1;
        }
        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador) {
            ganador = 1;
        }
    }

    // Verificar diagonales
    if (!ganador) {
        if (tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador) {
            ganador = 1;
        } else if (tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador) {
            ganador = 1;
        }
    }

    return ganador;
}

int empate(char tablero[FILAS][COLUMNAS]) {
    int lleno = 1;
    int i, j;

    for (i = 0; i < FILAS && lleno; i++) {
        for (j = 0; j < COLUMNAS && lleno; j++) {
            if (tablero[i][j] == ' ') {
                lleno = 0;
            }
        }
    }

    return lleno;
}

void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? p1 : p2;
        turnoJugador(tablero, &fila, &columna, actual);
        tablero[fila][columna] = actual;
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, (turno % 2) + 1);
    }
}

void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador) {
            turnoJugador(tablero, &fila, &columna, actual);
        } else {
            do {
                fila = rand() % 3;
                columna = rand() % 3;
            } while (movimientoInvalido(tablero, fila, columna));
        }

        tablero[fila][columna] = actual;
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, 0);
    }
}

void modoMedio(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador) {
            turnoJugador(tablero, &fila, &columna, actual);
        } else {
            int bloqueado = 0;
            int i, j;

            // Buscar movimiento para bloquear al jugador
            for (i = 0; i < FILAS && !bloqueado; i++) {
                for (j = 0; j < COLUMNAS && !bloqueado; j++) {
                    if (tablero[i][j] == ' ') {
                        tablero[i][j] = jugador;
                        if (hayGanador(tablero, jugador)) {
                            tablero[i][j] = maquina;
                            fila = i;
                            columna = j;
                            bloqueado = 1;
                        } else {
                            tablero[i][j] = ' ';
                        }
                    }
                }
            }

            // Si no hay movimiento para bloquear, mover aleatoriamente
            if (!bloqueado) {
                do {
                    fila = rand() % 3;
                    columna = rand() % 3;
                } while (movimientoInvalido(tablero, fila, columna));
            }
        }

        tablero[fila][columna] = actual;
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, 0);
    }
}

int evaluarEstado(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int puntaje = 0;

    if (hayGanador(tablero, maquina)) {
        puntaje = 10;
    } else if (hayGanador(tablero, jugador)) {
        puntaje = -10;
    }

    return puntaje;
}

int minimax(char tablero[FILAS][COLUMNAS], int profundidad, int esMaximizador, char jugador, char maquina) {
    int puntaje = evaluarEstado(tablero, jugador, maquina);
    int resultado = 0;
    int i, j;

    if (puntaje == 10 || puntaje == -10 || empate(tablero)) {
        resultado = puntaje;
    } else if (esMaximizador) {
        int mejor = -1000;
        int valor;

        for (i = 0; i < FILAS; i++) {
            for (j = 0; j < COLUMNAS; j++) {
                if (tablero[i][j] == ' ') {
                    tablero[i][j] = maquina;
                    valor = minimax(tablero, profundidad + 1, 0, jugador, maquina);
                    tablero[i][j] = ' ';
                    if (valor > mejor) {
                        mejor = valor;
                    }
                }
            }
        }
        resultado = mejor;
    } else {
        int peor = 1000;
        int valor;

        for (i = 0; i < FILAS; i++) {
            for (j = 0; j < COLUMNAS; j++) {
                if (tablero[i][j] == ' ') {
                    tablero[i][j] = jugador;
                    valor = minimax(tablero, profundidad + 1, 1, jugador, maquina);
                    tablero[i][j] = ' ';
                    if (valor < peor) {
                        peor = valor;
                    }
                }
            }
        }
        resultado = peor;
    }

    return resultado;
}

void modoDificil(char tablero[FILAS][COLUMNAS], char jugador, char maquina) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador) {
            turnoJugador(tablero, &fila, &columna, actual);
        } else {
            int mejorValor = -1000;
            int valor;
            int i, j;

            for (i = 0; i < FILAS; i++) {
                for (j = 0; j < COLUMNAS; j++) {
                    if (tablero[i][j] == ' ') {
                        tablero[i][j] = maquina;
                        valor = minimax(tablero, 0, 0, jugador, maquina);
                        tablero[i][j] = ' ';
                        if (valor > mejorValor) {
                            mejorValor = valor;
                            fila = i;
                            columna = j;
                        }
                    }
                }
            }
        }

        tablero[fila][columna] = actual;
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, 0);
    }
}

int verificarEstadoJuego(char tablero[FILAS][COLUMNAS], char actual, int *juegoTerminado, int *turno, int esJugador) {
    int victoria = hayGanador(tablero, actual);
    int empateJuego = empate(tablero);
    int resultado = 0;

    if (victoria || empateJuego) {
        mostrarTablero(tablero);
        if (victoria) {
            if (esJugador == 1 || esJugador == 2) {
                printf("Jugador %d (%c) gana!\n", esJugador, actual);
            } else {
                printf("%c gana!\n", actual);
            }
        } else {
            printf("Empate!\n");
        }
        *juegoTerminado = 1;
        resultado = 1;
    } else {
        (*turno)++;
        resultado = 0;
    }

    return resultado;
}
