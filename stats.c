#include <stdio.h>
#include <stdlib.h>
#include "stats.h"

void mostrarEstadisticas(stEstadisticas stats)
{
    printf("\n=== ESTADÍSTICAS ===\n");
    printf("Partidas jugadas: %d\n", stats.partidasJugadas);
    printf("Victorias Jugador 1: %d\n", stats.victoriasP1);
    printf("Victorias Jugador 2: %d\n", stats.victoriasP2);
    printf("Empates: %d\n", stats.empates);
}
