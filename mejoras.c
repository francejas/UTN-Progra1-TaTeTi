///ANTES
void jugarPvP(char tablero[FILAS][COLUMNAS], char p1, char p2)
{
    int turno = 0, fila, columna;
    char actual;

    while (1)
    {
        mostrarTablero(tablero);
         // turno % 2 siempre devuelve 0 o 1, si devuelve 0 le toca a p1, sino a p2.
        // turno % 2 es igual a 0 ? si si p1 sino p2
        // operador ternario o expresión condicional.
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
            printf("Jugador %d (%c) gana!\n", (turno % 2) + 1, actual);
            break;
        }

        if (empate(tablero))
        {
            mostrarTablero(tablero);
            printf("Empate!\n");
            break;
        }

        turno++;
    }
}

void modoFacil(char tablero[FILAS][COLUMNAS], char jugador, char maquina)
{
    int turno = 0;
    char actual;
    int fila, columna;

    while (1)
    {
        mostrarTablero(tablero);
        actual = (turno % 2 == 0) ? jugador : maquina;

        if (turno % 2 == 0)
        {
            printf("Turno del Jugador (%c)\n", actual);
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
        }
        else
        {
            printf("Turno de la Maquina (%c)...\n", actual);
            do
            {
                fila = rand() % 3;
                columna = rand() % 3;
            } while (movimientoInvalido(tablero, fila, columna));
        }

        tablero[fila][columna] = actual;

        if (hayGanador(tablero, actual))
        {
            mostrarTablero(tablero);
            printf("%c gana!\n", actual);
            break;
        }

        if (empate(tablero))
        {
            mostrarTablero(tablero);
            printf("Empate!\n");
            break;
        }

        turno++;
    }
}


///DESPUES


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