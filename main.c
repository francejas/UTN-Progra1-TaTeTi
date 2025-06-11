#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "stats.h"

#define FILAS 3
#define COLUMNAS 3

void inicializarTablero(char tablero[FILAS][COLUMNAS]);
int mostrarMenu();
void seleccionPiezas(char *p1, char *p2);
void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2);
void jugarModoFacil(char tablero[FILAS][COLUMNAS], char jugador);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);
int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna);
int hayGanador(char tablero[FILAS][COLUMNAS], char jugador);
int empate(char tablero[FILAS][COLUMNAS]);
int modoDificultad();
void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina);
void modoMedio();
void modoDificil();
void turnoJugador(char tablero[FILAS][COLUMNAS], int *fila, int *columna, char actual);

int main()
{
    srand(time(NULL));
    char tablero[FILAS][COLUMNAS];
    char p1, p2;
    int continuar = 1;

    stEstadisticas stats;

    while (continuar)
    {
        int modo = mostrarMenu();
        inicializarTablero(tablero);

        if (modo == 1)
        {
            seleccionPiezas(&p1, &p2);
            jugarPvP(tablero, p1, p2);
        }
        else
        {
            p1 = 'X';
            p2 = 'O';
            int dificultad = modoDificultad();
            if (dificultad == 1)
            {
                modoFacil(tablero, p1, p2);
            }
            else if (dificultad == 2)
            {
                modoMedio();
            }
            else
            {
                modoDificil();
            }
        }

        char respuesta;
        printf("Desea jugar otra partida? (s/n): ");
        scanf(" %c", &respuesta);
        if (respuesta != 's' && respuesta != 'S')
            continuar = 0;
    }

    //mostrarEstadisticas

    return 0;
}

void inicializarTablero(char tablero[FILAS][COLUMNAS])
{
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            tablero[i][j] = ' ';
}

int mostrarMenu()
{
    int opcion;
    int flag = 1;

    while (flag)
    {
        printf("=== TA-TE-TI ===\n");
        printf("1 - Jugador vs Jugador\n");
        printf("2 - Jugador vs Maquina\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion == 1 || opcion == 2)
            flag = 0;
        else
            printf("Opcion invalida. Intente de nuevo.\n");
    }

    system("cls");
    return opcion;
}

int modoDificultad()
{
    int dificultad;
    int flag = 1;

    while (flag)
    {
        printf("=== TA-TE-TI ===\n");
        printf("Seleccione dificultad:\n");
        printf("1 - Facil\n");
        printf("2 - Medio (Proximamente)\n");
        printf("3 - Dificil (Proximamente\n");
        printf("Eleccion: ");

        scanf("%d", &dificultad);
        if (dificultad == 1 || dificultad == 2 || dificultad == 3)
            flag = 0;
        else
            printf("Opcion invalida. Intente de nuevo.\n");
    }

    system("cls");
    return dificultad;
}

void seleccionPiezas(char *p1, char *p2)
{
    int flag = 1;

    while (flag)
    {
        printf("Jugador 1, elija su ficha (X o O): ");
        scanf(" %c", p1);
        *p1 = toupper(*p1);
        if (*p1 == 'X' || *p1 == 'O')
            flag = 0;
        else
            printf("Ficha invalida. Intente de nuevo.\n");
    }

    //Si *p1 es 'X', entonces *p2 será 'O'. Si no, entonces *p2 será 'X'.
    //condición ? valor_si_verdadero : valor_si_falso

    *p2 = (*p1 == 'X') ? 'O' : 'X';

     /*
    Es lo mismo que escribir:
    if (*p1 == 'X')
    {
        *p2 = 'O';
    }
    else
    {
        *p2 = 'X';
    }
    */

    printf("Jugador 1: %c | Jugador 2: %c\n", *p1, *p2);
}

void mostrarTablero(char tablero[FILAS][COLUMNAS])
{
    printf("\n   0   1   2\n");
    for (int i = 0; i < FILAS; i++)
    {
        printf("%d ", i);  // Número de fila al principio
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf(" %c ", tablero[i][j]);
            if (j < COLUMNAS - 1) printf("|");
        }
        printf("\n");
        if (i < FILAS - 1) printf("  ---+---+---\n");
    }
    printf("\n");
}

void turnoJugador(char tablero[FILAS][COLUMNAS], int *fila, int *columna, char actual)
{
    int flag = 1;

    printf("Turno del Jugador (%c)\n", actual);
    while (flag)
    {
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

void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2)
{
    int turno = 0, fila, columna;
    char actual;
    int juegoTerminado = 0;

    while (!juegoTerminado)
    {
        mostrarTablero(tablero);
         // turno % 2 siempre devuelve 0 o 1, si devuelve 0 le toca a p1, sino a p2.
        // turno % 2 es igual a 0 ? si si p1 sino p2
        // operador ternario o expresión condicional.
        actual = (turno % 2 == 0) ? p1 : p2;

        // Llamada a la función modularizada para el turno del jugador
        turnoJugador(tablero, &fila, &columna, actual);

        tablero[fila][columna] = actual;

        if (hayGanador(tablero, actual))
        {
            mostrarTablero(tablero);
            printf("Jugador %d (%c) gana!\n", (turno % 2) + 1, actual);
            juegoTerminado = 1;
        }
        else if (empate(tablero))
        {
            mostrarTablero(tablero);
            printf("Empate!\n");
            juegoTerminado = 1;
        }
        else
        {
            turno++;
        }
    }
}

void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina)
{
    int turno = 0;
    char actual;
    int fila, columna;
    int juegoTerminado = 0;

    while (!juegoTerminado)
    {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (turno % 2 == 0)
        {
            // Llamada a la función modularizada para el turno del jugador
            turnoJugador(tablero, &fila, &columna, actual);
        }
        else
        {
            printf("Turno de la Maquina (%c)...\n", actual);
            int movimientoValido = 0;
            while (!movimientoValido)
            {
                fila = rand() % 3;
                columna = rand() % 3;
                if (!movimientoInvalido(tablero, fila, columna))
                    movimientoValido = 1;
            }
        }

        tablero[fila][columna] = actual;

        if (hayGanador(tablero, actual))
        {
            mostrarTablero(tablero);
            printf("%c gana!\n", actual);
            juegoTerminado = 1;
        }
        else if (empate(tablero))
        {
            mostrarTablero(tablero);
            printf("Empate!\n");
            juegoTerminado = 1;
        }
        else
        {
            turno++;
        }
    }
}

int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna)
{
    //1 si invalid
    //0 si es valido

    int flag = 1;
    if (fila >= 0 && fila < FILAS && columna >= 0 && columna < COLUMNAS && tablero[fila][columna] == ' ')
    {
        flag = 0;
    }
    return flag;
}

int hayGanador(char tablero[FILAS][COLUMNAS], char jugador)
{
    int flag = 0;

    for (int i = 0; i < FILAS && !flag; i++)
    {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador)
            flag = 1;

        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador)
            flag = 1;
    }

    if (!flag && tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador)
        flag = 1;

    if (!flag && tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador)
        flag = 1;

    return flag;
}

int empate(char tablero[FILAS][COLUMNAS])
{
    int flag = 1;
    for (int i = 0; i < FILAS && flag; i++)
        for (int j = 0; j < COLUMNAS && flag; j++)
            if (tablero[i][j] == ' ')
                flag = 0;
    return flag;
}

void modoMedio()
{
    printf("Modo medio seleccionado\n");
}

void modoDificil()
{
    printf("Modo difícil seleccionado\n");
}
