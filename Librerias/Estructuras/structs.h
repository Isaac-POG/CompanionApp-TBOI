#ifndef STRUCTS_H
#define STRUCTS_H

//Estructura de los personajes
typedef struct tipoPersonaje
{
    int desbloqueado; //Indica si esta desbloqueado o no
    char nombre[13]; //Indica el nombre del personaje
    int marcas[10]; //Indica las marcas de logros, en el caso de que no lo ha conseguido(0), si lo consiguio en dificultad normal(1) o dificil(2).

}tipoPersonaje;

//Estructura de los items
typedef struct tipoItem
{
    int ID; //Indica el id del item
    int encontrado; //Indica si el item fue encontrado o no.
    char nombre[25]; //Indica el nombre del item
    char efecto[300]; //Indica una descripcion de lo que hace el item
    char tipoEfecto[7]; //Indica si es un activo o pasivo
}tipoItem;

//Estructura de los logros
typedef struct tipoLogro
{
    int ID; //Indica el id del logro
    int desbloqueado; //Indica si el logro fue desbloqueado o no
    char nombre[40]; //Indica el nombre del logro
    char descripcion[200]; //indica una descripcion del logro
    char comoConseguir[200]; //Indica como conseguir el logro
}tipoLogro;

//Estructura de los enemigos
typedef struct tipoEnemigo
{
    int ID; //Indica el id del enemigo
    int encontrado; //Indica si el enemigo fue encontrado
    char nombre[40]; //Indica el nombre del enemigo
    int vida; //Indica la vida que tiene el enemigo
    char ubicacion[240][240]; //Indica la(s) ubicacion(es) que tiene el enemigo en los pisos
    int cantidadUbicacion; //Indica una cuenta de cuantos pisos se encuentra el enemigo
}tipoEnemigo;

#endif
