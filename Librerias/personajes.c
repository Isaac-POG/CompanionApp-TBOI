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

                for(int j = 0; j < 10; j++)
                {
                    mostrarMarcas(j);
                    if(aux->marcas[j] == 0) printf(red"NO ");
                    else if(aux->marcas[j] == 1) printf(blue"NORMAL ");
                    else printf(green"DIFICIL ");
                }
            }
            aux = nextMap(mapaPersonajes);
        }
        printf(reset"\n");
    }
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
    tipoPersonaje * aux = searchMap(mapaPersonajes, nombrePersonaje);
    int  opcion;
    char respuesta[10];

    if(aux != NULL)
    {
        if(aux->desbloqueado == 1)
        {
            mostrarPersonaje(aux->nombre, aux->marcas);
            do
            {
                printf("\nHa logrado una nueva marca (SI/NO): ");
                scanf("%9s", respuesta);
                convertirMayuscula(respuesta);
            } while (strcmp(respuesta, "SI") != 0 && strcmp(respuesta,"NO") != 0);
            
            if(strcmp(respuesta, "SI") == 0)
            {
                printf("\n");
                for(int i = 0; i < 10; i++)
                {
                    printf(yellow"%i) ", i + 1);
                    mostrarMarcas(i);
                    printf(reset"\n");
                }

                do
                {
                    printf(reset"\nCual marca logro: ");
                    scanf("%i", &opcion);
                    if(opcion < 1 || opcion > 10) printf(red"\nNo existe tal marca\n"reset);
                } while (opcion < 1 || opcion > 10);
                
                if(aux->marcas[opcion - 1] == 2) printf(red"\nLa marca ya se logro\n"reset);
                else
                {
                    
                    do
                    {
                        printf("\nLo realizo en " blue"NORMAL " reset"O "blue"DIFICIL"reset": ");
                        scanf("%9s", respuesta);
                        convertirMayuscula(respuesta);
                        if(strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0) printf(red"\nNo existe tal opcion\n"reset);
                    } while (strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0);
                    
                    if(strcmp("NORMAL", respuesta) == 0) aux->marcas[opcion - 1] = 1;
                    else aux->marcas[opcion - 1] = 2;
                }
                
                mostrarPersonaje(aux->nombre, aux->marcas);
            }
        }
        else
        {
            printf(red"\nEl personaje %s no se encuentra desbloqueado\n"reset, aux->nombre);
        }
    }
    else
    {
        printf(red"\nEl personaje ingresado no existe\n"reset);
        return;
    }
}