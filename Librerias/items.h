#ifndef ITEMS_H
#define ITEMS_H

//Funcion para calcular la cantidad de items encontrados
float calculoTotalItems(List *);

//Importa el archivo items.txt y almacena su informacion dentro un mapa
void importarArchivoItems(HashMap *, List *);

//Guarda la informacion actualizada durante la ejecucion del programa dentro del archivo items.txt
void guardarInfoItems(List *);

//Busca un Item a través de su nombre dentro del mapaItems e indica su información
void buscarItemEspecifico(HashMap *, char *);

//Muestra todos los items del juego, indicando si el usuario los encontro o no
void mostrarTodosItems(List *);

//Busca un item que el usuario haya encontrado, para actualizar el estado del item
void encontrarItem(HashMap *, char *);

#endif