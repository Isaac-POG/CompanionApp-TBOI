#ifndef PERSONAJES_H
#define PERSONAJES_H

//Funcion para calcular el total del avance del jugador respecto a los personajes
float calculoTotalPersonajes(List *);

//Se importa el archivo personajes.txt y almacena su informacion en un mapa
void importarArchivoPersonajes(HashMap *, List *);

//Se guardan los cambios realizados durante la ejecucion del programa, y estos cambios se guardan en personajes.txt
void guardarInfoPersonajes(List *);

//Funcion que muestra a todos los personajes, ordenados de la misma forma que en el juego
void mostrarPersonajes(HashMap *, List *);

//Función que actualiza la informacion de desbloqueo de un personaje
void desbloquearPersonajes(List *, HashMap *);

//Funcion para actualizar la informacion respecto al avance en las marcas de logro de un personaje
void avanceMarcasLogros(List *, HashMap *);

#endif