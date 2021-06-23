#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

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

void importarArchivoPersonajes(HashMap * mapaPersonajes)
{
    FILE * archivo = fopen("Archivos/characters.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[100];

    fgets(lineaLeida, 99, archivo);

    while(fgets(lineaLeida, 99, archivo))
    {
        tipoPersonaje * nuevoPersonaje = copiarInformacionPersonaje(lineaLeida);
        insertMap(mapaPersonajes, nuevoPersonaje->nombre, nuevoPersonaje);
    } 

    fclose(archivo);
}

void desbloquearPersonajes(HashMap * mapaPersonajes, char * nombrePersonaje)
{
    initscr();
    tipoPersonaje * aux = searchMap(mapaPersonajes, nombrePersonaje);
    if(aux != NULL)
    {
        if(aux->desbloqueado != 1)
        {
            aux->desbloqueado = 1;
            printw("\n%s ahora esta desbloqueado!\n", aux->nombre);
        }
        else
        {
            printw("\nEl personaje ingresado ya se encuentra desbloqueado\n");
        }
    }
    else
    {
        printw("\nEl personaje ingresado no existe\n");
    }
    getch();
    endwin();
}

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
                printw("%s ", aux->nombre);
                for(int largo = strlen(aux->nombre) ; largo < 13 ; largo++){
                    printw(" ");
                }
                
                if(aux->desbloqueado == 0) printw("Bloqueado    ");
                else printw("Desbloqueado ");

                for(int j = 0; j < 10; j++)
                {
                    mostrarMarcas(j);
                    if(aux->marcas[j] == 0) printw("NO      ");
                    else if(aux->marcas[j] == 1) printw("NORMAL  ");
                    else printw("DIFICIL ");
                }
            }
            aux = nextMap(mapaPersonajes);
        }
        printw("\n");
    }
    getch();
    endwin();
}

void guardarInfoPersonajes(HashMap * mapaPersonajes)
{
    FILE * archivo = fopen("characters.txt", "w");
    if(archivo == NULL) return;
    fprintf(archivo,"ID,Personaje,Desbloqueado,Marcas\n");
    
    for(int i = 1; i < 16; i++)
    {
        tipoPersonaje * aux = firstMap(mapaPersonajes);
        while(aux != NULL)
        {    
            if(i == aux->ID)
            {
                fprintf(archivo, "%i,%s,%i", aux->ID, aux->nombre, aux->desbloqueado);
                for(int j = 0; j < 10; j++)
                {
                    fprintf(archivo, ",%i", aux->marcas[j]);
                }
            }
            aux = nextMap(mapaPersonajes);
        }
        fprintf(archivo,"\n");
    }
    fclose(archivo);
}

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
                    printw("\nLo realizo en " "NORMAL " "O ""DIFICIL"": ");
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
            printw("\nEl personaje %s no se encuentra desbloqueado\n", aux->nombre);
        }
    }
    else
    {
        printw("\nEl personaje ingresado no existe\n");
    }
    getch();
    endwin();
}