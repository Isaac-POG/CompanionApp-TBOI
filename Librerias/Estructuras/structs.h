#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct tipoPersonaje
{
    int ID;
    char nombre[13];
    int desbloqueado;
    int marcas[10];

}tipoPersonaje;

typedef struct tipoItem
{
    int ID;
    char nombre[40];
    int encontrado;
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
    int nombre;
    char ubicacion[50];
    int encontrado;
}tipoEnemigo;

#endif