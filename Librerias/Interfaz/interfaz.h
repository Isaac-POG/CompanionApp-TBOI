#ifndef INTERFAZ_H
#define INTERFAZ_H

//Funcion que delimita el tamaño del menu a usar
WINDOW * crearVentana(int cantOpciones);
void mostrarMarcas();
int valorNumericoMarca(char * nombreMarca);
void convertirMayuscula(char *);
void mostrarPersonaje(char *, int *);
void pantallaInicial();
void pantallaFinal();
void iniciarColores();
void esperarTecla();
#endif