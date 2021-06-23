#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"


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

	fragmento = strtok(NULL, ";");
	strcpy(aux->tipoEfecto, fragmento);
	return aux;
}

void importarArchivoItems(HashMap * mapaItems)
{
	FILE * archivo = fopen("Archivos/items.txt", "r");
	if(archivo == NULL){printw("HOA\n"); return;}

	char lineaLeida[400];
	fgets(lineaLeida, 399, archivo);
	while(fgets(lineaLeida, 399, archivo))
	{
		tipoItem * nuevoItem = copiarInformacionItems(lineaLeida);
		insertMap(mapaItems, nuevoItem->nombre, nuevoItem);
	}

	fclose(archivo);
}

void mostrarTodosItems(HashMap * mapaItems)
{
	initscr();
	scrollok(stdscr, TRUE);
	noecho();
	printw("\n");
	for(int i = 1; i < 553; i++)
	{
		tipoItem * aux = firstMap(mapaItems);
		while(aux != NULL)
		{
			if(aux->ID == i)
			{
				wprintw(stdscr,"%s ", aux->nombre);
				if(aux->encontrado == 0) wprintw(stdscr,"No encontrado \n");
				else wprintw(stdscr,"Encontrado \n" );
			}
			aux = nextMap(mapaItems);
		}
		if(i % 38 == 0)
		{
			printw("\nIngrese cualquier tecla para avanzar\n");
			getch();
			clear();
			wrefresh(stdscr);
		}
	}
	echo();
	getch();
	endwin();
}

void encontrarItem(HashMap * mapaItems, char * nombreItem)
{
	initscr();
	tipoItem * itemBuscado = searchMap(mapaItems, nombreItem);

	if(itemBuscado != NULL)
	{
		if(itemBuscado->encontrado == 0)
		{
			printw("\nSe actualizo informacion del item\n");
			printw("%s ENCONTRADO\n", nombreItem);
			itemBuscado->encontrado = 1;
		}
		else
		{
			printw("El item %s ya se habia encontrado", nombreItem);
		}
	}
	else
	{
		printw("\nEl item con nombre %s no existe\n", nombreItem);
	}
	getch();
	endwin();
}

void buscarItemEspecifico(HashMap * mapaItems, char * nombreItem)
{
	initscr();
	tipoItem * itemBuscado = searchMap(mapaItems, nombreItem);
	
	if(itemBuscado != NULL)
	{
		printw("\n%s\n", itemBuscado->nombre);
		if(itemBuscado->encontrado == 0) printw("No encontrado ");
		else printw("Encontrado ");
		printw("Tipo de Objeto: ""%s\n", itemBuscado->tipoEfecto);
		printw("Efecto: ""%s\n", itemBuscado->efecto);
	}
	else
	{
		printw("\nEl item con nombre %s no existe\n", nombreItem);
	}
	getch();
	endwin();
}