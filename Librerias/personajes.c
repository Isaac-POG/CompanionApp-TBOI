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

	//Posicion X
	fragmento = strtok(lineaLeida, ",");
	nuevoPersonaje->ID = strtol(fragmento, NULL, 10);

	//Posicion Y
	fragmento = strtok(NULL, ",");
	strcpy(nuevoPersonaje->nombre, fragmento);

	//ID
	fragmento = strtok(NULL, ",");
	nuevoPersonaje->desbloqueado = strtol(fragmento, NULL, 10);

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