#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

tipoPersonaje * copiarInformacion(char * lineaLeida)
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
    FILE * archivo = fopen("characters.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[100];

    fgets(lineaLeida, 99, archivo);

    while(fgets(lineaLeida, 99, archivo))
    {
        tipoPersonaje * nuevoPersonaje = copiarInformacion(lineaLeida);
        insertMap(mapaPersonajes, nuevoPersonaje->nombre, nuevoPersonaje);
    } 

    fclose(archivo);
}

void desbloquearPersonajes(HashMap * mapaPersonajes, char * nombrePersonaje)
{
    tipoPersonaje * aux = searchMap(mapaPersonajes, nombrePersonaje);
    if(aux != NULL)
    {
        if(aux->desbloqueado != 1)
        {
            aux->desbloqueado = 1;
            printf(green"\n%s ahora esta desbloqueado!\n"reset, aux->nombre);
        }
        else
        {
            printf(red"\nEl personaje ingresado ya se encuentra desbloqueado\n"reset);
            return;
        }
    }
    else
    {
        printf(red"\nEl personaje ingresado no existe\n"reset);
        return;
    }
}

void mostrarPersonajes(HashMap * mapaPersonajes)
{
    for(int i = 1; i < 16; i++)
    {
        tipoPersonaje * aux = firstMap(mapaPersonajes);
        while(aux != NULL)
        {    
            if(i == aux->ID)
            {
                printf(yellow"%s ", aux->nombre);
                for(int largo = strlen(aux->nombre) ; largo < 13 ; largo++){
                    printf(" ");
                }
                
                if(aux->desbloqueado == 0) printf(red"Bloqueado    ");
                else printf(green"Desbloqueado ");

                for(int i = 0; i < 10; i++)
                {
                    mostrarMarcas(i);
                    if(aux->marcas[i] == 0) printf(red"NO ");
                    else printf(green"SI ");
                }
            }
            aux = nextMap(mapaPersonajes);
        }
        printf(reset"\n");
    }
}