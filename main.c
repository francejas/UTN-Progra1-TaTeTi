#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define FILAS 3
#define COLUMNAS 3

void inicializarTablero(char tablero[FILAS][COLUMNAS]);
int mostrarMenu();
void seleccionPiezas(char *p1, char *p2);
void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2);
void mostrarTablero(char tablero[FILAS][COLUMNAS]);
int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna);
int hayGanador(char tablero[FILAS][COLUMNAS], char jugador);
int empate(char tablero[FILAS][COLUMNAS]);
int modoDificultad();
void modoFacil();
void modoMedio();
void modoDificil();
int deseaSeguirJugando();

int main()
{
    srand(time(NULL));
    char tablero[FILAS][COLUMNAS];
    char p1, p2;

    while (1)
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
            int dificultad = modoDificultad();
            if (dificultad == 1)
            {
                modoFacil();
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

        if (!deseaSeguirJugando())
        {
            printf("Gracias por jugar.\n");
            break;
        }
    }

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
    do
    {
        printf("=== TA-TE-TI ===\n");
        printf("1 - Jugador vs Jugador\n");
        printf("2 - Jugador vs Maquina\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion != 1 && opcion != 2)
            printf("Opcion invalida. Intente de nuevo.\n");
    }
    while (opcion != 1 && opcion != 2);

    system("cls");
    return opcion;
}

int modoDificultad()
{
    int dificultad;
    do
    {
        printf("=== TA-TE-TI ===\n");
        printf("Seleccione dificultad:\n");
        printf("1 - Facil\n");
        printf("2 - Medio\n");
        printf("3 - Dificil\n");

        scanf("%d", &dificultad);
        if (dificultad != 1 && dificultad != 2 && dificultad != 3)
            printf("Opcion invalida. Intente de nuevo.\n");
    }
    while (dificultad != 1 && dificultad != 2 && dificultad != 3);

    system("cls");
    return dificultad;
}

void seleccionPiezas(char *p1, char *p2)
{
    do
    {
        printf("Jugador 1, elija su ficha (X o O): ");
        scanf(" %c", p1);
        *p1 = toupper(*p1);
        if (*p1 != 'X' && *p1 != 'O')
            printf("Ficha invalida. Intente de nuevo.\n");
    }
    while (*p1 != 'X' && *p1 != 'O');

    *p2 = (*p1 == 'X') ? 'O' : 'X';
    printf("Jugador 1: %c | Jugador 2: %c\n", *p1, *p2);
}

void mostrarTablero(char tablero[FILAS][COLUMNAS])
{
    printf("\n");
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            printf(" %c ", tablero[i][j]);
            if (j < COLUMNAS - 1) printf("|");
        }
        printf("\n");
        if (i < FILAS - 1) printf("---+---+---\n");
    }
    printf("\n");
}

void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2)
{
    int turno = 0, fila, columna;
    char actual;

    while (1)
    {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? p1 : p2;
        printf("Turno del Jugador %d (%c)\n", (turno % 2) + 1, actual);

        do
        {
            printf("Ingrese fila (0-2): ");
            scanf("%d", &fila);
            printf("Ingrese columna (0-2): ");
            scanf("%d", &columna);
            if (movimientoInvalido(tablero, fila, columna))
                printf("Movimiento invalido. Intente de nuevo.\n");
        }
        while (movimientoInvalido(tablero, fila, columna));

        tablero[fila][columna] = actual;

        if (hayGanador(tablero, actual))
        {
            mostrarTablero(tablero);
            printf("\u00a1Jugador %d (%c) gana!\n", (turno % 2) + 1, actual);
            break;
        }

        if (empate(tablero))
        {
            mostrarTablero(tablero);
            printf("\u00a1Empate!\n");
            break;
        }

        turno++;
    }
}

int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna)
{
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
    for (int i = 0; i < FILAS; i++)
    {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador)
            flag = 1;

        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador)
            flag = 1;
    }

    if (tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador)
        flag = 1;

    if (tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador)
        flag = 1;

    return flag;
}

int empate(char tablero[FILAS][COLUMNAS])
{
    int flag = 1;
    for (int i = 0; i < FILAS; i++)
        for (int j = 0; j < COLUMNAS; j++)
            if (tablero[i][j] == ' ')
                flag = 0;
    return flag;
}

void modoFacil()
{
    printf("Modo fácil seleccionado\n");
}

void modoMedio()
{
    printf("Modo medio seleccionado\n");
}

void modoDificil()
{
    printf("Modo difícil seleccionado\n");
}

int deseaSeguirJugando()
{
    char respuesta;
    do
    {
        printf("\n¿Desea jugar otra partida? (s/n): ");
        scanf(" %c", &respuesta);
        respuesta = tolower(respuesta);
        if (respuesta != 's' && respuesta != 'n')
            printf("Respuesta invalida. Ingrese 's' para sí o 'n' para no.\n");
    } while (respuesta != 's' && respuesta != 'n');

    return (respuesta == 's');
}
