#ifndef INTERFAZ_H
#define INTERFAZ_H

WINDOW * crearVentana(int cantOpciones);

void mostrarMarcas();

int valorNumericoMarca(char * nombreMarca);

void convertirMayuscula(char *);

void mostrarPersonaje(char *,int , int *, int);

void pantallaInicial();

void pantallaFinal();

void iniciarColores();

void esperarTecla(int);

void centrarEnY(int);

void centrarEnX(int);
#endif