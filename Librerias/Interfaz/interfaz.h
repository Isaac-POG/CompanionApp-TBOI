#ifndef INTERFAZ_H
#define INTERFAZ_H

#define cls "\e[1;1H\e[2J"
#define red "\e[1;91m"
#define green "\e[1;92m" 
#define yellow "\e[1;93m"
#define blue "\e[1;94m"
#define purple "\e[1;95m"
#define reset "\e[0m"

void mostrarMenuOpciones();
void mostrarMenuDesbloqueo();
void mostrarMarcas();
void esperarEnter();
void convertirMayuscula(char *);
void mostrarPersonaje(char *, int *);

#endif