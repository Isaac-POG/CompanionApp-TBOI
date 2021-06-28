#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct tipoPersonaje
{
    int ID;
    int desbloqueado;
    char nombre[13];
    int marcas[10];

}tipoPersonaje;

typedef struct tipoItem
{
    int ID;
    int encontrado;
    char nombre[25];
    char efecto[300];
    char tipoEfecto[7];
}tipoItem;

typedef struct tipoLogro
{
    int ID;
    int desbloqueado;
    char nombre[40];
    char descripcion[40];
    char comoConseguir[200];
}tipoLogro;

typedef struct tipoEnemigo
{
    int ID;
    int encontrado;
    char nombre[40];
    int vida;
    char ubicacion[240][240];
    int cantidadUbicacion;
}tipoEnemigo;

#endif
