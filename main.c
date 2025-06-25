#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILAS 3
#define COLUMNAS 3

#define AR_JUGADORES "jugadores.dat"
#define AR_PARTIDAS "partidas.dat"
#define AR_PARTIDAXJUGADOR "partidaXjugador.dat"

typedef struct {
    int idJugador;
    char nombre[30];
    char apellido[30];
    char email[50];
    char username[30];
    char password[20];
    char dni[10];
    int ptsTotales;
    int eliminado;
} stJugador;

typedef struct {
    int idPartida;
    int dificultad;
    int esContraCpu;
} stPartida;

typedef struct {
    int idPartidaJugador;
    int idPartida;
    int idJugador;
    int resultado;
    int puntosJugador;
} stPartidaXJugador;

void inicializarTablero(char tablero[FILAS][COLUMNAS]);
int mostrarMenuJuego();
void seleccionPiezas(char *p1, char *p2);
void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2, stJugador jugador1, stJugador jugador2);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);
int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna);
int hayGanador(char tablero[FILAS][COLUMNAS], char jugador);
int empate(char tablero[FILAS][COLUMNAS]);
int modoDificultad();
void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina, stJugador jugadorLogueado);
void modoMedio(char tablero[FILAS][COLUMNAS], char jugador, char maquina, stJugador jugadorLogueado);
void modoDificil(char tablero[FILAS][COLUMNAS], char jugador, char maquina, stJugador jugadorLogueado);
void turnoJugador(char tablero[FILAS][COLUMNAS], int *fila, int *columna, char actual);
int minimax(char tablero[FILAS][COLUMNAS], int profundidad, int esMaximizador, char jugador, char maquina);
int evaluarEstado(char tablero[FILAS][COLUMNAS], char jugador, char maquina);
int verificarEstadoJuego(char tablero[FILAS][COLUMNAS], char actual, int *juegoTerminado, int *turno, stJugador jugador1, stJugador jugador2, int esContraCpu, int dificultad, char p1, char p2);

int mostrarMenuPrincipal(stJugador jugadorLogueado);
stJugador iniciarSesion(char nombreArchivo[]);
void registrarJugador(char nombreArchivo[]);
stJugador cargarUnJugador(char nombreArchivo[]);
void mostrarRanking(char nombreArchivo[]);
int validarEmail(char email[], char nombreArchivo[], int idExcluir);
int validarUsername(char username[], char nombreArchivo[], int idExcluir);
int validarPassword(char password[]);
int obtenerUltimoId(char nombreArchivo[]);
void ordenarRanking(stJugador jugadores[], int validos);
void menuVerPerfil(stJugador* jugadorLogueado);
void modificarPerfil(stJugador* jugador, char nombreArchivo[]);
int actualizarJugadorEnArchivo(stJugador jugador, char nombreArchivo[]);
void eliminarPerfil(stJugador* jugador, char nombreArchivo[]);
void mostrarEstadisticas(stJugador jugador, char archivoPartidasXJugador[]);

void guardarPartida(stPartida partida, char nombreArchivo[]);
void guardarPartidaXJugador(stPartidaXJugador pxj, char nombreArchivo[]);
int obtenerUltimoIdPartida(char nombreArchivo[]);
int obtenerUltimoIdPartidaXJugador(char nombreArchivo[]);

int main() {
    srand(time(NULL));
    char tablero[FILAS][COLUMNAS];
    char p1, p2;
    int salir = 0;
    stJugador jugadorLogueado;
    jugadorLogueado.idJugador = -1;

    while (!salir) {
        int opcion = mostrarMenuPrincipal(jugadorLogueado);
        system("cls");

        switch(opcion) {
            case 1:
            {
                int modo = mostrarMenuJuego();
                inicializarTablero(tablero);

                if (modo == 1) {
                    seleccionPiezas(&p1, &p2);
                    stJugador jugador2;
                    jugador2.idJugador = -1;
                    if (jugadorLogueado.idJugador != -1) {
                        char respuesta;
                        printf("¿El jugador 2 desea iniciar sesión? (s/n): ");
                        scanf(" %c", &respuesta);
                        if (respuesta == 's' || respuesta == 'S') {
                            jugador2 = iniciarSesion(AR_JUGADORES);
                            if (jugador2.idJugador == -1) {
                                printf("No se encontró el email o la contraseña es incorrecta.\n");
                                printf("¿Desea registrarse? (s/n): ");
                                scanf(" %c", &respuesta);
                                if (respuesta == 's' || respuesta == 'S') {
                                    registrarJugador(AR_JUGADORES);
                                    printf("Registro exitoso. Ahora inicie sesión.\n");
                                    jugador2 = iniciarSesion(AR_JUGADORES);
                                }
                            }
                        }
                    }
                    jugarPvP(tablero, p1, p2, jugadorLogueado, jugador2);
                } else {
                    p1 = 'X';
                    p2 = 'O';
                    int dificultad = modoDificultad();
                    stJugador jugador2;
                    jugador2.idJugador = -1;
                    if (dificultad == 1) {
                        modoFacil(tablero, p1, p2, jugadorLogueado);
                    } else if (dificultad == 2) {
                        modoMedio(tablero, p1, p2, jugadorLogueado);
                    } else {
                        modoDificil(tablero, p1, p2, jugadorLogueado);
                    }
                }
                break;
            }
            case 2:
                if (jugadorLogueado.idJugador == -1) {
                    jugadorLogueado = iniciarSesion(AR_JUGADORES);
                    if (jugadorLogueado.idJugador != -1) {
                        printf("Inicio de sesión exitoso!\n");
                    } else {
                        printf("Email o contraseña incorrectos.\n");
                    }
                } else {
                    menuVerPerfil(&jugadorLogueado);
                }
                break;
            case 3:
                if (jugadorLogueado.idJugador == -1) {
                    registrarJugador(AR_JUGADORES);
                } else {
                    mostrarEstadisticas(jugadorLogueado, AR_PARTIDAXJUGADOR);
                }
                break;
            case 4:
                if (jugadorLogueado.idJugador != -1) {
                    mostrarRanking(AR_JUGADORES);
                } else {
                    salir = 1;
                }
                break;
            case 5:
                if (jugadorLogueado.idJugador != -1) {
                    jugadorLogueado.idJugador = -1;
                    printf("Sesión cerrada.\n");
                }
                break;
            case 6:
                if (jugadorLogueado.idJugador != -1) {
                    salir = 1;
                }
                break;
            default:
                break;
        }
        if (!salir) {
            printf("\nPresione ENTER para volver al menú principal...");
            getchar(); getchar();
            system("cls");
        }
    }

    printf("Gracias por jugar!\n");
    return 0;
}

int mostrarMenuPrincipal(stJugador jugadorLogueado) {
    int opcion = 0;
    int flag = 1;
    int resultado = 0;
    int maxOpcion = 4;

    while (flag) {
        printf("=== TA-TE-TI ===\n");
        if (jugadorLogueado.idJugador != -1) {
            printf("Bienvenido, %s!\n", jugadorLogueado.username);
            printf("-------------------\n");
            maxOpcion = 6;
        }

        if (jugadorLogueado.idJugador == -1) {
            printf("1 - Partida rápida\n");
        } else {
            printf("1 - Iniciar partida\n");
        }

        if (jugadorLogueado.idJugador == -1) {
            printf("2 - Iniciar sesión\n");
            printf("3 - Registrarse\n");
            printf("4 - Salir\n");
        } else {
            printf("2 - Ver/Modificar mi perfil\n");
            printf("3 - Ver mis estadísticas\n");
            printf("4 - Ranking de jugadores\n");
            printf("5 - Cerrar sesión\n");
            printf("6 - Salir del juego\n");
        }

        printf("Seleccione una opción: ");
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); // Limpia el búfer
            printf("Entrada inválida. Por favor, ingrese un número.\n");
        } else if (opcion >= 1 && opcion <= maxOpcion) {
            flag = 0;
            resultado = opcion;
        } else {
            printf("Opción inválida. Intente de nuevo.\n");
        }
        system("pause");
        system("cls");
    }
    return resultado;
}

void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2, stJugador jugador1, stJugador jugador2) {
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
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, jugador1, jugador2, 0, 0, p1, p2);
    }
}

int verificarEstadoJuego(char tablero[FILAS][COLUMNAS], char actual, int *juegoTerminado, int *turno, stJugador jugador1, stJugador jugador2, int esContraCpu, int dificultad, char p1, char p2) {
    int victoria = hayGanador(tablero, actual);
    int empateJuego = empate(tablero);
    int resultado = 0;

    if (victoria || empateJuego) {
        mostrarTablero(tablero);
        if (victoria) {
            if (esContraCpu) {
                if (actual == p1) {
                    printf("Jugador (%c) gana!\n", actual);
                } else {
                    printf("Máquina (%c) gana!\n", actual);
                }
            } else {
                if (actual == p1) {
                    printf("Jugador 1 (%c) gana!\n", actual);
                } else {
                    printf("Jugador 2 (%c) gana!\n", actual);
                }
            }
        } else {
            printf("Empate!\n");
        }
        *juegoTerminado = 1;
        resultado = 1;

        if (esContraCpu) {
            if (jugador1.idJugador != -1) {
                stPartida partida;
                partida.idPartida = obtenerUltimoIdPartida(AR_PARTIDAS) + 1;
                partida.dificultad = dificultad;
                partida.esContraCpu = 1;
                guardarPartida(partida, AR_PARTIDAS);

                stPartidaXJugador pxj;
                pxj.idPartidaJugador = obtenerUltimoIdPartidaXJugador(AR_PARTIDAXJUGADOR) + 1;
                pxj.idPartida = partida.idPartida;
                pxj.idJugador = jugador1.idJugador;
                if (victoria && actual == p1) {
                    pxj.resultado = 1;
                    pxj.puntosJugador = 3;
                } else if (empateJuego) {
                    pxj.resultado = 2;
                    pxj.puntosJugador = 1;
                } else {
                    pxj.resultado = 0;
                    pxj.puntosJugador = 0;
                }
                guardarPartidaXJugador(pxj, AR_PARTIDAXJUGADOR);
                jugador1.ptsTotales += pxj.puntosJugador;
                actualizarJugadorEnArchivo(jugador1, AR_JUGADORES);
            }
        } else {
            if (jugador1.idJugador != -1 && jugador2.idJugador != -1) {
                stPartida partida;
                partida.idPartida = obtenerUltimoIdPartida(AR_PARTIDAS) + 1;
                partida.dificultad = 0;
                partida.esContraCpu = 0;
                guardarPartida(partida, AR_PARTIDAS);

                stPartidaXJugador pxj1;
                pxj1.idPartidaJugador = obtenerUltimoIdPartidaXJugador(AR_PARTIDAXJUGADOR) + 1;
                pxj1.idPartida = partida.idPartida;
                pxj1.idJugador = jugador1.idJugador;
                if (victoria && actual == p1) {
                    pxj1.resultado = 1;
                    pxj1.puntosJugador = 3;
                } else if (empateJuego) {
                    pxj1.resultado = 2;
                    pxj1.puntosJugador = 1;
                } else {
                    pxj1.resultado = 0;
                    pxj1.puntosJugador = 0;
                }
                guardarPartidaXJugador(pxj1, AR_PARTIDAXJUGADOR);

                stPartidaXJugador pxj2;
                pxj2.idPartidaJugador = pxj1.idPartidaJugador + 1;
                pxj2.idPartida = partida.idPartida;
                pxj2.idJugador = jugador2.idJugador;
                if (victoria && actual == p2) {
                    pxj2.resultado = 1;
                    pxj2.puntosJugador = 3;
                } else if (empateJuego) {
                    pxj2.resultado = 2;
                    pxj2.puntosJugador = 1;
                } else {
                    pxj2.resultado = 0;
                    pxj2.puntosJugador = 0;
                }
                guardarPartidaXJugador(pxj2, AR_PARTIDAXJUGADOR);

                jugador1.ptsTotales += pxj1.puntosJugador;
                jugador2.ptsTotales += pxj2.puntosJugador;
                actualizarJugadorEnArchivo(jugador1, AR_JUGADORES);
                actualizarJugadorEnArchivo(jugador2, AR_JUGADORES);
            } else if (jugador1.idJugador != -1) {
                stPartida partida;
                partida.idPartida = obtenerUltimoIdPartida(AR_PARTIDAS) + 1;
                partida.dificultad = 0;
                partida.esContraCpu = 0;
                guardarPartida(partida, AR_PARTIDAS);

                stPartidaXJugador pxj;
                pxj.idPartidaJugador = obtenerUltimoIdPartidaXJugador(AR_PARTIDAXJUGADOR) + 1;
                pxj.idPartida = partida.idPartida;
                pxj.idJugador = jugador1.idJugador;
                if (victoria && actual == p1) {
                    pxj.resultado = 1;
                    pxj.puntosJugador = 3;
                } else if (empateJuego) {
                    pxj.resultado = 2;
                    pxj.puntosJugador = 1;
                } else {
                    pxj.resultado = 0;
                    pxj.puntosJugador = 0;
                }
                guardarPartidaXJugador(pxj, AR_PARTIDAXJUGADOR);
                jugador1.ptsTotales += pxj.puntosJugador;
                actualizarJugadorEnArchivo(jugador1, AR_JUGADORES);
            }
        }
    } else {
        (*turno)++;
        resultado = 0;
    }

    return resultado;
}

void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina, stJugador jugadorLogueado) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;
    stJugador jugador2;
    jugador2.idJugador = -1;

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
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, jugadorLogueado, jugador2, 1, 1, jugador, maquina);
    }
}

void modoMedio(char tablero[FILAS][COLUMNAS], char jugador, char maquina, stJugador jugadorLogueado) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;
    stJugador jugador2;
    jugador2.idJugador = -1;

    while (!juegoTerminado) {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (actual == jugador) {
            turnoJugador(tablero, &fila, &columna, actual);
        } else {
            int bloqueado = 0;
            int i, j;

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

            if (!bloqueado) {
                do {
                    fila = rand() % 3;
                    columna = rand() % 3;
                } while (movimientoInvalido(tablero, fila, columna));
            }
        }

        tablero[fila][columna] = actual;
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, jugadorLogueado, jugador2, 1, 2, jugador, maquina);
    }
}

void modoDificil(char tablero[FILAS][COLUMNAS], char jugador, char maquina, stJugador jugadorLogueado) {
    int turno = 0;
    int fila = 0;
    int columna = 0;
    char actual;
    int juegoTerminado = 0;
    stJugador jugador2;
    jugador2.idJugador = -1;

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
        verificarEstadoJuego(tablero, actual, &juegoTerminado, &turno, jugadorLogueado, jugador2, 1, 3, jugador, maquina);
    }
}

int mostrarMenuJuego() {
    int opcion = 0;
    int flag = 1;
    int resultado = 0;

    while (flag) {
        printf("=== TA-TE-TI ===\n");
        printf("=== MODO DE JUEGO ===\n");
        printf("1 - Jugador vs Jugador\n");
        printf("2 - Jugador vs Máquina\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        if (opcion == 1 || opcion == 2) {
            flag = 0;
            resultado = opcion;
        } else {
            printf("Opción inválida. Intente de nuevo.\n");
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
        printf("1 - Fácil\n");
        printf("2 - Medio\n");
        printf("3 - Difícil\n");
        printf("Elección: ");
        scanf("%d", &dificultad);
        if (dificultad >= 1 && dificultad <= 3) {
            flag = 0;
            resultado = dificultad;
        } else {
            printf("Opción inválida. Intente de nuevo.\n");
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
            printf("Ficha inválida. Intente de nuevo.\n");
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
            printf("Movimiento inválido. Intente de nuevo.\n");
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

    for (i = 0; i < FILAS && !ganador; i++) {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador) {
            ganador = 1;
        }
        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador) {
            ganador = 1;
        }
    }

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

void menuVerPerfil(stJugador* jugadorLogueado) {
    int opcion = 0;
    int salir = 0;

    while(!salir) {
        system("cls");
        printf("--- GESTION DE PERFIL ---\n");
        printf("Usuario: %s\n", jugadorLogueado->username);
        printf("-------------------------\n");
        printf("1. Modificar mis datos\n");
        printf("2. Eliminar mi cuenta\n");
        printf("0. Volver al menu principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                modificarPerfil(jugadorLogueado, AR_JUGADORES);
                break;
            case 2:
                eliminarPerfil(jugadorLogueado, AR_JUGADORES);
                if(jugadorLogueado->idJugador == -1) {
                    salir = 1;
                }
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
        if(!salir) {
            system("pause");
        }
    }
}

void modificarPerfil(stJugador* jugador, char nombreArchivo[]) {
    int opcion = 0;
    char nuevaPass[20];
    int actualizado = 0;
    stJugador copiaJugador = *jugador;

    system("cls");
    printf("--- MODIFICAR DATOS ---\n");
    printf("¿Que dato desea modificar?\n");
    printf("1. Nombre\n");
    printf("2. Apellido\n");
    printf("3. Email\n");
    printf("4. Username\n");
    printf("5. Contrasena\n");
    printf("6. DNI\n");
    printf("0. Cancelar\n");
    printf("Opcion: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            printf("Nuevo Nombre: ");
            scanf("%s", copiaJugador.nombre);
            break;
        case 2:
            printf("Nuevo Apellido: ");
            scanf("%s", copiaJugador.apellido);
            break;
        case 3:
            printf("Nuevo Email: ");
            scanf("%s", copiaJugador.email);
            if(!validarEmail(copiaJugador.email, nombreArchivo, copiaJugador.idJugador)){
                printf("Error: Email invalido o ya en uso por otro jugador.\n");
                return;
            }
            break;
        case 4:
            printf("Nuevo Username: ");
            scanf("%s", copiaJugador.username);
            if(!validarUsername(copiaJugador.username, nombreArchivo, copiaJugador.idJugador)){
                printf("Error: Username ya en uso por otro jugador.\n");
                return;
            }
            break;
        case 5:
            printf("Nueva Contrasena: ");
            scanf("%s", nuevaPass);
            if(!validarPassword(nuevaPass)){
                printf("Error: La contrasena no cumple los requisitos (mayuscula y minuscula).\n");
                return;
            }
            strcpy(copiaJugador.password, nuevaPass);
            break;
        case 6:
            printf("Nuevo DNI: ");
            scanf("%s", copiaJugador.dni);
            break;
        case 0:
            return;
        default:
            printf("Opcion invalida.\n");
            return;
    }

    actualizado = actualizarJugadorEnArchivo(copiaJugador, nombreArchivo);

    if(actualizado) {
        *jugador = copiaJugador;
        printf("Dato actualizado correctamente.\n");
    } else {
        printf("Error: No se pudo actualizar el dato en el archivo.\n");
    }
}

int actualizarJugadorEnArchivo(stJugador jugador, char nombreArchivo[]) {
    int actualizado = 0;
    FILE* fp = fopen(nombreArchivo, "r+b");

    if (fp == NULL) {
        printf("Error al abrir el archivo de jugadores.\n");
    } else {
        stJugador temp;
        while(fread(&temp, sizeof(stJugador), 1, fp) > 0 && !actualizado) {
            if(temp.idJugador == jugador.idJugador) {
                fseek(fp, -sizeof(stJugador), SEEK_CUR);
                fwrite(&jugador, sizeof(stJugador), 1, fp);
                actualizado = 1;
            }
        }
        fclose(fp);
    }
    return actualizado;
}

void eliminarPerfil(stJugador* jugador, char nombreArchivo[]) {
    char passwordConfirm[20];
    int eliminado = 0;

    printf("--- ELIMINAR CUENTA ---\n");
    printf("ATENCION: Esta accion es irreversible.\n");
    printf("Para confirmar, ingrese su contrasena: ");
    scanf("%s", passwordConfirm);

    if(strcmp(jugador->password, passwordConfirm) == 0) {
        jugador->eliminado = 1;
        eliminado = actualizarJugadorEnArchivo(*jugador, nombreArchivo);
        if(eliminado) {
            printf("Su cuenta ha sido eliminada exitosamente. Se cerrara la sesion.\n");
            jugador->idJugador = -1;
        } else {
            printf("Error: No se pudo procesar la eliminacion.\n");
            jugador->eliminado = 0;
        }
    } else {
        printf("Contrasena incorrecta. La eliminacion ha sido cancelada.\n");
    }
}

void mostrarEstadisticas(stJugador jugador, char archivoPartidasXJugador[]) {
    int jugadas = 0, ganadas = 0, empatadas = 0, derrotas = 0;
    float porcGanadas = 0, porcEmpatadas = 0, porcDerrotas = 0;
    stPartidaXJugador pxj;

    FILE* fp = fopen(archivoPartidasXJugador, "rb");

    if (fp == NULL) {
        printf("Aun no has jugado ninguna partida.\n");
    } else {
        while(fread(&pxj, sizeof(stPartidaXJugador), 1, fp) > 0) {
            if (pxj.idJugador == jugador.idJugador) {
                jugadas++;
                if (pxj.resultado == 1) {
                    ganadas++;
                } else if (pxj.resultado == 2) {
                    empatadas++;
                } else {
                    derrotas++;
                }
            }
        }
        fclose(fp);
    }

    if(jugadas > 0) {
        porcGanadas = ((float)ganadas / jugadas) * 100;
        porcEmpatadas = ((float)empatadas / jugadas) * 100;
        porcDerrotas = ((float)derrotas / jugadas) * 100;
    }

    printf("--- MIS ESTADISTICAS ---\n");
    printf("Username: %s\n", jugador.username);
    printf("---------------------------------------------------\n");
    printf("Partidas jugadas: %d\n", jugadas);
    printf("---------------------------------------------------\n");
    printf("Partidas ganadas: %d (%.2f%%)\n", ganadas, porcGanadas);
    printf("Partidas empatadas: %d (%.2f%%)\n", empatadas, porcEmpatadas);
    printf("Partidas perdidas: %d (%.2f%%)\n", derrotas, porcDerrotas);
    printf("---------------------------------------------------\n");
}

stJugador iniciarSesion(char nombreArchivo[]) {
    char email[50];
    char password[20];
    stJugador jugador;
    stJugador jugadorEncontrado;
    jugadorEncontrado.idJugador = -1;
    int encontrado = 0;

    FILE* fp = fopen(nombreArchivo, "rb");

    if (fp == NULL) {
        printf("Error al abrir el archivo de jugadores. Puede que no haya jugadores registrados.\n");
    } else {
        printf("--- INICIO DE SESION ---\n");
        printf("Email: ");
        scanf("%s", email);
        printf("Contrasena: ");
        scanf("%s", password);

        while (fread(&jugador, sizeof(stJugador), 1, fp) > 0 && !encontrado) {
            if (strcmp(jugador.email, email) == 0 && strcmp(jugador.password, password) == 0 && jugador.eliminado == 0) {
                encontrado = 1;
                jugadorEncontrado = jugador;
            }
        }
        fclose(fp);
    }
    return jugadorEncontrado;
}

void registrarJugador(char nombreArchivo[]) {
    stJugador nuevoJugador = cargarUnJugador(nombreArchivo);

    if (nuevoJugador.idJugador != -1) {
        FILE* fp = fopen(nombreArchivo, "ab");
        if (fp == NULL) {
            printf("Error critico: No se pudo abrir el archivo para registrar al jugador.\n");
        } else {
            fwrite(&nuevoJugador, sizeof(stJugador), 1, fp);
            fclose(fp);
            printf("\n¡Registro exitoso! Ahora puede iniciar sesion.\n");
        }
    } else {
        printf("\nEl proceso de registro fue cancelado.\n");
    }
}

stJugador cargarUnJugador(char nombreArchivo[]) {
    stJugador jugador;
    int esValido = 0;

    printf("--- REGISTRO DE NUEVO JUGADOR ---\n");
    jugador.idJugador = -1;

    do {
        printf("Ingrese Email (debe contener @ y .com): ");
        scanf("%s", jugador.email);
        esValido = validarEmail(jugador.email, nombreArchivo, -1);
        if(!esValido){
            printf("Error: El formato del email es invalido o ya esta en uso.\n");
        }
    } while (!esValido);

    do {
        printf("Ingrese Contrasena (al menos 1 mayuscula y 1 minuscula): ");
        scanf("%s", jugador.password);
        esValido = validarPassword(jugador.password);
        if(!esValido){
            printf("Error: La contrasena no cumple con los requisitos.\n");
        }
    } while (!esValido);

    do {
        printf("Ingrese Username (debe ser unico): ");
        scanf("%s", jugador.username);
        esValido = validarUsername(jugador.username, nombreArchivo, -1);
        if(!esValido){
            printf("Error: El username ya esta en uso.\n");
        }
    } while (!esValido);

    printf("Ingrese Nombre: ");
    scanf("%s", jugador.nombre);
    printf("Ingrese Apellido: ");
    scanf("%s", jugador.apellido);
    printf("Ingrese DNI: ");
    scanf("%s", jugador.dni);

    jugador.idJugador = obtenerUltimoId(nombreArchivo) + 1;
    jugador.ptsTotales = 0;
    jugador.eliminado = 0;

    return jugador;
}

int validarEmail(char email[], char nombreArchivo[], int idExcluir) {
    int resultado = 0;
    stJugador jugador;
    int existe = 0;
    FILE* fp;

    if (strchr(email, '@') != NULL && strstr(email, ".com") != NULL) {
        fp = fopen(nombreArchivo, "rb");
        if (fp != NULL) {
            while(fread(&jugador, sizeof(stJugador), 1, fp) > 0 && !existe){
                if(strcmp(jugador.email, email) == 0 && jugador.eliminado == 0 && jugador.idJugador != idExcluir){
                    existe = 1;
                }
            }
            fclose(fp);
        }
        if(!existe){
            resultado = 1;
        }
    }
    return resultado;
}

int validarUsername(char username[], char nombreArchivo[], int idExcluir) {
    stJugador jugador;
    int existe = 0;
    int resultado = 0;
    FILE* fp = fopen(nombreArchivo, "rb");

    if (fp != NULL) {
        while(fread(&jugador, sizeof(stJugador), 1, fp) > 0 && !existe){
            if(strcmp(jugador.username, username) == 0 && jugador.eliminado == 0 && jugador.idJugador != idExcluir){
                existe = 1;
            }
        }
        fclose(fp);
    }

    if (!existe) {
        resultado = 1;
    }

    return resultado;
}

int validarPassword(char password[]) {
    int tieneMayus = 0;
    int tieneMinus = 0;
    int i = 0;
    int resultado = 0;

    while (password[i] != '\0') {
        if (isupper(password[i])) {
            tieneMayus = 1;
        } else if (islower(password[i])) {
            tieneMinus = 1;
        }
        i++;
    }

    if (tieneMayus && tieneMinus) {
        resultado = 1;
    }

    return resultado;
}

int obtenerUltimoId(char nombreArchivo[]) {
    stJugador jugador;
    int ultimoId = 0;
    FILE* fp = fopen(nombreArchivo, "rb");

    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) > 0) {
            fseek(fp, -sizeof(stJugador), SEEK_END);
            if (fread(&jugador, sizeof(stJugador), 1, fp) > 0) {
                ultimoId = jugador.idJugador;
            }
        }
        fclose(fp);
    }
    return ultimoId;
}

void mostrarRanking(char nombreArchivo[]) {
    stJugador jugadores[100];
    stJugador jugador;
    int validos = 0;
    FILE* fp = fopen(nombreArchivo, "rb");

    if (fp == NULL) {
        printf("Aun no hay jugadores registrados para mostrar un ranking.\n");
    } else {
        while(fread(&jugador, sizeof(stJugador), 1, fp) > 0 && validos < 100){
            if(jugador.eliminado == 0){
                jugadores[validos] = jugador;
                validos++;
            }
        }
        if (fread(&jugador, sizeof(stJugador), 1, fp) > 0) {
            printf("Nota: Solo se muestran los primeros 100 jugadores debido a limitaciones de memoria.\n");
        }
        fclose(fp);

        if (validos > 0) {
            ordenarRanking(jugadores, validos);

            printf("--- RANKING DE JUGADORES ---\n");
            printf("------------------------------\n");
            printf("| Puesto | Username   | Puntos |\n");
            printf("------------------------------\n");
            for(int i = 0; i < validos; i++){
                printf("| %-6d | %-10s | %-6d |\n", i + 1, jugadores[i].username, jugadores[i].ptsTotales);
            }
            printf("------------------------------\n");
        } else {
            printf("No hay jugadores activos para mostrar en el ranking.\n");
        }
    }
}

void ordenarRanking(stJugador jugadores[], int validos) {
    int i, j;
    stJugador aux;
    for (i = 0; i < validos - 1; i++) {
        for (j = 0; j < validos - i - 1; j++) {
            if (jugadores[j].ptsTotales < jugadores[j + 1].ptsTotales) {
                aux = jugadores[j];
                jugadores[j] = jugadores[j+1];
                jugadores[j+1] = aux;
            }
        }
    }
}

void guardarPartida(stPartida partida, char nombreArchivo[]) {
    FILE* fp = fopen(nombreArchivo, "ab");
    if (fp == NULL) {
        printf("Error crítico: No se pudo abrir el archivo %s para guardar la partida.\n", nombreArchivo);
        return;
    }
    fwrite(&partida, sizeof(stPartida), 1, fp);
    fclose(fp);
}

void guardarPartidaXJugador(stPartidaXJugador pxj, char nombreArchivo[]) {
    FILE* fp = fopen(nombreArchivo, "ab");
    if (fp == NULL) {
        printf("Error crítico: No se pudo abrir el archivo %s para guardar la partida por jugador.\n", nombreArchivo);
        return;
    }
    fwrite(&pxj, sizeof(stPartidaXJugador), 1, fp);
    fclose(fp);
}

int obtenerUltimoIdPartida(char nombreArchivo[]) {
    stPartida partida;
    int ultimoId = 0;
    FILE* fp = fopen(nombreArchivo, "rb");

    if (fp != NULL) {
        fseek(fp, -sizeof(stPartida), SEEK_END);
        if (fread(&partida, sizeof(stPartida), 1, fp) > 0) {
            ultimoId = partida.idPartida;
        }
        fclose(fp);
    }
    return ultimoId;
}

int obtenerUltimoIdPartidaXJugador(char nombreArchivo[]) {
    stPartidaXJugador pxj;
    int ultimoId = 0;
    FILE* fp = fopen(nombreArchivo, "rb");

    if (fp != NULL) {
        fseek(fp, -sizeof(stPartidaXJugador), SEEK_END);
        if (fread(&pxj, sizeof(stPartidaXJugador), 1, fp) > 0) {
            ultimoId = pxj.idPartidaJugador;
        }
        fclose(fp);
    }
    return ultimoId;
}
