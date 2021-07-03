#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "TDA_Lista/list.h"
#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

//Copia la informacion de la lineaLeida a un tipoItem
tipoItem * copiarInformacionItems(char * lineaLeida)
{
	tipoItem * aux = malloc (sizeof(tipoItem));
	char * fragmento;
	
	//ID
	fragmento = strtok(lineaLeida, ";");
	aux->ID = strtol(fragmento, NULL, 10);
	
	//Encontrado
	fragmento = strtok(NULL, ";");
	aux->encontrado = strtol(fragmento, NULL, 10);
	
	//Nombre
	fragmento = strtok(NULL, ";");
	strcpy(aux->nombre, fragmento);
	
	//Efecto
	fragmento = strtok(NULL, ";");
	strcpy(aux->efecto, fragmento);

	//Tipo de Item
	fragmento = strtok(NULL, "\n");
	strcpy(aux->tipoEfecto, fragmento);
	
	return aux;
}

//Importa el archivo items.txt y almacena su informacion dentro un mapa
void importarArchivoItems(HashMap * mapaItems, List * listaItems)
{
	//Se busca el archivo
	FILE * archivo = fopen("Archivos/items.txt", "r");
	if(archivo == NULL) return;

	char lineaLeida[400];

	//Se elimina la primera linea del archivo
	fgets(lineaLeida, 399, archivo);
	
	//Se copia la informacion de los items en el mapa y la lista
	while(fgets(lineaLeida, 399, archivo))
	{
		tipoItem * nuevoItem = copiarInformacionItems(lineaLeida);
		insertMap(mapaItems, nuevoItem->nombre, nuevoItem);
		pushBack(listaItems, nuevoItem);
	}

	//Se cierra el archivo
	fclose(archivo);
}

//Guarda la informacion actualizada durante la ejecucion del programa dentro del archivo items.txt
void guardarInfoItems(List * listaItems)
{
	//Se abre el archivo que contiene la informacion de los items
	FILE * archivo = fopen("Archivos/items.txt", "w");
    if(archivo == NULL) return;

	//Se copia la primera linea
    fprintf(archivo,"Id;Encontrado;Nombre;Efecto;Tipo\n");
    
	//Se recorre la lista para copiar la informacion
    tipoItem * aux = firstList(listaItems);
    while(aux != NULL)
    {    
        fprintf(archivo, "%i;%i;%s;%s;%s\n", aux->ID, aux->encontrado, aux->nombre, aux->efecto, aux->tipoEfecto);
        aux = nextList(listaItems);
    }

	//Se cierra el archivo
    fclose(archivo);
}

//Busca un Item a través de su nombre dentro del mapaItems e indica su información
void buscarItemEspecifico(HashMap * mapaItems, char * nombreItem)
{
	initscr();
	tipoItem * itemBuscado = searchMap(mapaItems, nombreItem);
	
	//Se busca el item, si existe se muestra, sino, se indica que el item no existe
	if(itemBuscado != NULL)
	{
		printw("\n%s\n", itemBuscado->nombre);
		if(itemBuscado->encontrado == 0)
		{
			attron(COLOR_PAIR(3));
			wprintw(stdscr,"No encontrado \n");
			attroff(COLOR_PAIR(3));
		} 
		else
		{
			attron(COLOR_PAIR(2));
			wprintw(stdscr,"Encontrado \n");
			attroff(COLOR_PAIR(2));
		}
		printw("Tipo de Objeto: ""%s\n", itemBuscado->tipoEfecto);
		printw("Efecto: ""%s\n", itemBuscado->efecto);
	}
	else
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(3));
		printw("\nEl item con nombre %s no existe\n", nombreItem);
		attroff(A_BOLD);
		attroff(COLOR_PAIR(3));
	}

	esperarTecla();
	endwin();
}

//Muestra todos los items del juego, indicando si el usuario los encontro o no
void mostrarTodosItems(List * listaItems)
{
	//Inicio del Ncurses.h
	initscr();

	//Activar el scroll
	scrollok(stdscr, TRUE);

	tipoItem * aux = firstList(listaItems);
	int i = 1;

	printw("Nombre                 Encontrado\n");			
	
	//Se recorre la lista hasta que se llegue al final
	while(aux != NULL)
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(4));

		wprintw(stdscr,"%s ",aux->nombre);

		attroff(COLOR_PAIR(4));
		attroff(A_BOLD);

		if(strlen(aux->nombre) < 22)
		{
            for(int k = strlen(aux->nombre); k < 22; k++) wprintw(stdscr," ");
        }

		//Diferenciar entre si el usuario encontro o no el item
		if(aux->encontrado == 0)
		{
			attron(COLOR_PAIR(3));
			wprintw(stdscr,"No encontrado \n");
			attroff(COLOR_PAIR(3));
		} 
		else
		{
			attron(COLOR_PAIR(2));
			wprintw(stdscr,"Encontrado \n" );
			attroff(COLOR_PAIR(2));
		}

		//Pausa para poder ver los items, la cantidad depende del tamaño de la terminal
		if(i % (stdscr->_maxy - 2) == 0 || i == 546)
		{
			wrefresh(stdscr);
			esperarTecla();
			printw("Nombre                 Encontrado\n");
		}

		aux = nextList(listaItems);
		i++;
	}

	endwin();
}

//Busca un item que el usuario haya encontrado, para actualizar el estado del item
void encontrarItem(HashMap * mapaItems, char * nombreItem)
{
	initscr();
	tipoItem * itemBuscado = searchMap(mapaItems, nombreItem);

	//Se busca el item en el mapa, si existe se actualiza la informacion, si no se le indica al usuario que no existe
	if(itemBuscado != NULL)
	{
		if(itemBuscado->encontrado == 0)
		{
			printw("\nSe actualizo informacion del item a\n");
			attron(A_BOLD);
			attron(COLOR_PAIR(2));
			printw("%s ENCONTRADO\n", nombreItem);
			attroff(A_BOLD);
			attroff(COLOR_PAIR(2));
			itemBuscado->encontrado = 1;
		}
		else
		{
			attron(COLOR_PAIR(4));
			printw("\nEl item %s ya se habia encontrado", nombreItem);
			attroff(COLOR_PAIR(4));
		}
	}
	else
	{
		attron(COLOR_PAIR(3));
		printw("\nEl item con nombre %s no existe\n", nombreItem);
		attroff(COLOR_PAIR(3));
	}

	esperarTecla();
	endwin();
}