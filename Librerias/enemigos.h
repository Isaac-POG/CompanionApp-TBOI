#ifndef ENEMIGOS_H
#define ENEMIGOS_H

//Calcula el porcentaje avanzado en enemigos
float calculoTotalEnemigos(List * );

//Importa el archivo enemigos
void importarArchivoEnemigos(HashMap *, List *);

//Busca un enemigo ingresado por el usuario
void buscarEnemigoEspecifico(HashMap *, char *);

//Muestra todos los enemigos
void mostrarEnemigos(List *);

//El usuario indica si encontro el enemigo
void encontrarEnemigo(HashMap *, char *);

//Se guarda toda la informacion de los enemigos
void guardarInfoEnemigos(List *);

#endif