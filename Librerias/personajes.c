#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Mapa/hashmap.h"
#include "TDA_Lista/list.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

//Se separa la informacion del personaje y se almacena en un tipoPersonaje
tipoPersonaje * copiarInformacionPersonaje(char * lineaLeida)
{
    tipoPersonaje * nuevoPersonaje = malloc (sizeof(tipoPersonaje));
	char * fragmento;

    //ID Personaje
	fragmento = strtok(lineaLeida, ",");
	nuevoPersonaje->ID = strtol(fragmento, NULL, 10);

	//Nombre Personaje
	fragmento = strtok(NULL, ",");
	strcpy(nuevoPersonaje->nombre, fragmento);

	//Personaje desbloqueado
	fragmento = strtok(NULL, ",");
	nuevoPersonaje->desbloqueado = strtol(fragmento, NULL, 10);

    //Marcas del Personaje
    for(int i = 0; i < 10; i++)
    {
        fragmento = strtok(NULL, ",");
	    nuevoPersonaje->marcas[i] = strtol(fragmento, NULL, 10);
    }

	return nuevoPersonaje;
}

//Se importa el archivo personajes.txt y almacena su informacion en un mapa
void importarArchivoPersonajes(HashMap * mapaPersonajes, List * listaPersonajes)
{
    FILE * archivo = fopen("Archivos/personajes.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[100];

    fgets(lineaLeida, 99, archivo);

    while(fgets(lineaLeida, 99, archivo))
    {
        tipoPersonaje * nuevoPersonaje = copiarInformacionPersonaje(lineaLeida);
        insertMap(mapaPersonajes, nuevoPersonaje->nombre, nuevoPersonaje);
        pushBack(listaPersonajes, nuevoPersonaje);
    } 

    fclose(archivo);
}

//Se guardan los cambios realizados durante la ejecucion del programa, y estos cambios se guardan en personajes.txt
void guardarInfoPersonajes(List * listaPersonajes)
{
    FILE * archivo = fopen("Archivos/personajes.txt", "w");
    if(archivo == NULL) return;
    
    fprintf(archivo,"ID,Personaje,Desbloqueado,Marcas\n");
    tipoPersonaje * aux = firstList(listaPersonajes);

    for(int i = 1; i < 16; i++)
    {
        fprintf(archivo, "%i,%s,%i", aux->ID, aux->nombre, aux->desbloqueado);
        for(int j = 0; j < 10; j++)
        {
            fprintf(archivo, ",%i", aux->marcas[j]);
        }
        fprintf(archivo,"\n");
        aux = nextList(listaPersonajes);
    }
    fclose(archivo);
}

//Funcion que muestra a todos los personajes, ordenados de la misma forma que en el juego
void mostrarPersonajes(HashMap * mapaPersonajes)
{
    initscr();
    for(int i = 1; i < 16; i++)
    {
        tipoPersonaje * aux = firstMap(mapaPersonajes);
        while(aux != NULL)
        {    
            if(i == aux->ID)
            {
                attron(A_BOLD);
                printw("%s ", aux->nombre);
                attroff(A_BOLD);
                for(int largo = strlen(aux->nombre) ; largo < 13 ; largo++){
                    printw(" ");
                }
                
                if(aux->desbloqueado == 0)
                {
                    attron(COLOR_PAIR(3));
                    attron(A_BOLD);
                    printw("Bloqueado    ");
                    attroff(COLOR_PAIR(3));
                    attroff(A_BOLD);
                } 
                else
                {
                    attron(COLOR_PAIR(2));
                    attron(A_BOLD);
                    printw("Desbloqueado ");
                    attroff(COLOR_PAIR(2));
                    attroff(A_BOLD);
                } 

                for(int j = 0; j < 10; j++)
                {
                    mostrarMarcas(j);
                    if(aux->marcas[j] == 0)
                    {
                        attron(COLOR_PAIR(3));
                        printw("NO      ");
                        attroff(COLOR_PAIR(3));
                    } 
                    else if(aux->marcas[j] == 1)
                    {
                        attron(COLOR_PAIR(4));
                        printw("NORMAL  ");
                        attroff(COLOR_PAIR(4));
                    }
                    else 
                    {
                        attron(COLOR_PAIR(2));
                        printw("DIFICIL ");
                        attroff(COLOR_PAIR(2));
                    }
                }
            }
            aux = nextMap(mapaPersonajes);
        }
        printw("\n");
    }
    getch();
    endwin();
}

//Función que actualiza la informacion de desbloqueo de un personaje
void desbloquearPersonajes(HashMap * mapaPersonajes, char * nombrePersonaje)
{
    initscr();

    tipoPersonaje * aux = searchMap(mapaPersonajes, nombrePersonaje);
    
    if(aux != NULL)
    {
        if(aux->desbloqueado != 1)
        {
            aux->desbloqueado = 1;
            attron(COLOR_PAIR(2));
            attron(A_BOLD);
            printw("\n%s ahora esta desbloqueado!\n", aux->nombre);
            attroff(COLOR_PAIR(2));
            attroff(A_BOLD);
        }
        else
        {
            attron(COLOR_PAIR(4));
            printw("\nEl personaje ingresado ya se encuentra desbloqueado\n");
            attroff(COLOR_PAIR(4));
        }
    }
    else
    {
        attron(COLOR_PAIR(3));
        printw("\nEl personaje ingresado no existe\n");
        attroff(COLOR_PAIR(3));
    }

    getch();
    
    endwin();
}

//Funcion para actualizar la informacion respecto al avance en las marcas de logro de un personaje
void avanceMarcasLogros(HashMap * mapaPersonajes, char * nombrePersonaje)
{
    initscr();
    tipoPersonaje * aux = searchMap(mapaPersonajes, nombrePersonaje);
    int  opcion;
    char respuesta[20];

    if(aux != NULL)
    {
        if(aux->desbloqueado == 1)
        {
            mostrarPersonaje(aux->nombre, aux->marcas);

            do
            {
                printw("\nCual marca logro: ");
                scanw("%19[^\n]s", respuesta);
                convertirMayuscula(respuesta);
                opcion = valorNumericoMarca(respuesta);
                if(opcion < 0 || opcion > 9) printw("\nNo existe tal marca\n");
            } while (opcion < 0 || opcion > 9);
            
            if(aux->marcas[opcion - 1] == 2) printw("\nLa marca ya se logro\n");
            else
            {
                
                do
                {
                    printw("\nLo realizo en ""NORMAL ""O ""DIFICIL"": ");
                    scanw("%19s", respuesta);
                    convertirMayuscula(respuesta);
                    if(strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0) printw("\nNo existe tal opcion\n");
                } while (strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0);
                
                if(strcmp("NORMAL", respuesta) == 0) aux->marcas[opcion] = 1;
                else aux->marcas[opcion] = 2;
            }
                
            mostrarPersonaje(aux->nombre, aux->marcas);
        }
        else
        {
            attron(COLOR_PAIR(4));
            printw("\nEl personaje %s no se encuentra desbloqueado\n", aux->nombre);
            attroff(COLOR_PAIR(4));
        }
    }
    else
    {
        attron(COLOR_PAIR(3));
        attron(A_BOLD);
        printw("\nEl personaje ingresado no existe\n");
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
    }
    getch();
    endwin();
}