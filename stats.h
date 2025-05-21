#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

typedef struct {
    int partidasJugadas;
    int victoriasP1;
    int victoriasP2;
    int empates;
} stEstadisticas;

//Prototipado
void mostrarEstadisticas(stEstadisticas stats);


#endif // STATS_H_INCLUDED
