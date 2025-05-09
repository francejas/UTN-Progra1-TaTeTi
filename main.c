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
int esMovimientoValido(char tablero[FILAS][COLUMNAS], int fila, int columna);


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

        //else if (modo == 2) => jugador vs máquina
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

void seleccionPiezas(char *p1, char *p2)
{
    do
    {
        printf("Jugador 1, elija su ficha (X o O): ");
        scanf(" %c", p1);
        //evitar minusculas
        *p1 = toupper(*p1);
        if (*p1 != 'X' && *p1 != 'O')
            printf("Ficha invalida. Intente de nuevo.\n");
    }
    while (*p1 != 'X' && *p1 != 'O');

    //Si *p1 es 'X', entonces *p2 será 'O'. Si no, entonces *p2 será 'X'.
    //condición ? valor_si_verdadero : valor_si_falso;

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

int movimientoInvalido(char tablero[FILAS][COLUMNAS], int fila, int columna)
{
    //aca deveria de retornar 1 si es invalido
}


void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2)
{
    int turno = 0, fila, columna;
    //jugador actual
    char actual;

    while (1)
    {
        mostrarTablero(tablero);

        // turno % 2 siempre devuelve 0 o 1, si devuelve 0 le toca a p1, sino a p2.
        // turno % 2 es igual a 0 ? si si p1 sino p2

        actual = (turno % 2 == 0) ? p1 : p2;
        printf("Turno del Jugador %d (%c)\n", (turno % 2) + 1, actual);

        do
        {
            printf("Ingrese fila (0-2): ");
            scanf("%d", &fila);
            printf("Ingrese columna (0-2): ");
            scanf("%d", &columna);
            //validacion de casillero libre y que no este fuera de los limites
            if (movimientoInvalido(tablero, fila, columna))
                printf("Movimiento invalido. Intente de nuevo.\n");
        }
        while (!movimientoInvalido(tablero, fila, columna));

        tablero[fila][columna] = actual;

        //valida ganador
        if (hayGanador(tablero, actual))
        {
            mostrarTablero(tablero);
            printf("¡Jugador %d (%c) gana!\n", (turno % 2) + 1, actual);
            break;
        }

        //valida empate
        if (tableroLleno(tablero))
        {
            mostrarTablero(tablero);
            printf("¡Empate!\n");
            break;
        }

        turno++;
    }
}

int hayGanador(){
}

int tableroLleno(){
}
