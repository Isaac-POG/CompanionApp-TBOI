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

	//Tipo de Item
	fragmento = strtok(NULL, "\n");
	strcpy(aux->tipoEfecto, fragmento);
	
	return aux;
}

void importarArchivoItems(HashMap * mapaItems)
{
	//Se busca el archivo
	FILE * archivo = fopen("Archivos/items.txt", "r");
	if(archivo == NULL) return;

	char lineaLeida[400];

	//Se elimina la primera linea del archivo
	fgets(lineaLeida, 399, archivo);
	
	//Se copia la informacion de los items
	while(fgets(lineaLeida, 399, archivo))
	{
		tipoItem * nuevoItem = copiarInformacionItems(lineaLeida);
		insertMap(mapaItems, nuevoItem->nombre, nuevoItem);
	}

	//Se cierra el archivo
	fclose(archivo);
}

void mostrarTodosItems(HashMap * mapaItems)
{
	//Inicio del Ncurses.h
	initscr();

	//Activar el scroll
	scrollok(stdscr, TRUE);

	for(int i = 1; i < 553; i++)
	{
		tipoItem * aux = firstMap(mapaItems);
		while(aux != NULL)
		{
			if(aux->ID == i)
			{
				attron(A_BOLD);
				attron(COLOR_PAIR(4));
				wprintw(stdscr,"%s ", aux->nombre);
				attroff(COLOR_PAIR(4));
				attroff(A_BOLD);

				//Diferenciar entre si se encontro el item o no
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
			}
			aux = nextMap(mapaItems);
		}
		if(i % 20 == 0 || i == 552)
		{
			printw("\nIngrese cualquier tecla para avanzar");
			getch();
			clear();
			wrefresh(stdscr);
		}
	}
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
	getch();
	endwin();
}

void guardarInfoItems(HashMap * mapaItems)
{
	FILE * archivo = fopen("Archivos/items.txt", "w");
    if(archivo == NULL) return;

    fprintf(archivo,"ID;ENCONTRADO;NOMBRE;EFECTO;TIPO\n");
    
    for(int i = 1; i < 553; i++)
    {
        tipoItem * aux = firstMap(mapaItems);
        while(aux != NULL)
        {    
            if(i == aux->ID)
            {
                fprintf(archivo, "%i;%i;%s;%s;%s\n", aux->ID, aux->encontrado, aux->nombre, aux->efecto, aux->tipoEfecto);
            }
            aux = nextMap(mapaItems);
        }
    }
    fclose(archivo);
}