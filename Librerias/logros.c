#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

tipoLogro * copiarInformacionLogro(char * lineaLeida)
{
    tipoLogro * nuevoLogro = malloc (sizeof(tipoLogro));
	char * fragmento;

    //ID logro
	fragmento = strtok(lineaLeida, ";");
	nuevoLogro->ID = strtol(fragmento, NULL, 10);

    //Logro desbloqueado
	fragmento = strtok(NULL, ";");
	nuevoLogro->desbloqueado = strtol(fragmento, NULL, 10);

	//Nombre logro
	fragmento = strtok(NULL, ";");
	strcpy(nuevoLogro->nombre, fragmento);

    //Descripcion del logro
	fragmento = strtok(NULL, ";");
	strcpy(nuevoLogro->descripcion, fragmento);

    //Como conseguir el logro
	fragmento = strtok(NULL, ";");
	strcpy(nuevoLogro->comoConseguir, fragmento);

	return nuevoLogro;
}

void importarArchivoLogros(HashMap * mapaLogros){
    FILE * archivo = fopen("logros.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[200];

    fgets(lineaLeida, 199, archivo);

    while(fgets(lineaLeida, 199, archivo))
    {
        tipoLogro * nuevoLogro = copiarInformacionLogro(lineaLeida);
        insertMap(mapaLogros, &nuevoLogro->ID, nuevoLogro);
    } 

    fclose(archivo);
}

void mostrarLogros(HashMap * mapaLogros){
    tipoLogro * aux = firstMap(mapaLogros);
    
    for(int j=1; j<404 ; j++){
        aux = firstMap(mapaLogros);
        while(aux){
            if(j == aux->ID){
                printf(yellow"ID: %d ",aux->ID);
                if(aux->ID < 10) printf("  ");
                else if(aux->ID < 100) printf(" ");

                if(aux->desbloqueado){
                    printf(green"Desbloqueado");
                }else{
                    printf(red"Bloqueado   ");
                }
                printf(cian" -> %s\n"reset,aux->nombre);
                break;
            }
            
            aux = nextMap(mapaLogros);
        }
    }
}

void buscarLogroEspecifico(HashMap * mapaLogros, int id){
    tipoLogro * aux = searchMap(mapaLogros,&id);
    if(!aux){
        printf("El logro que ingreso no existe\n");
        return;
    }
    printf(yellow"ID: %d ",aux->ID);

    if(aux->desbloqueado){
        printf(green"Desbloqueado");
    }else{
        printf(red"Bloqueado");
    }
    printf(cian"\nNombre: %s",aux->nombre);
    printf(blue"\nDescripcion: %s\n",aux->descripcion);
    printf(green"Como conseguir:\n%s"reset,aux->comoConseguir);

}

void desbloquearLogro(HashMap * mapaLogros, int id){
    tipoLogro * aux = searchMap(mapaLogros,&id);
    if(!aux){
        printf("El logro que ingreso no existe\n");
        return;
    }
    if(aux->desbloqueado){
        printf(red"\nEl logro ya estaba desbloqueado de antes\n"reset);
        return;
    }
    aux->desbloqueado = 1;

    printf(yellow"ID: %d ",aux->ID);

    if(aux->desbloqueado){
        printf(green"Desbloqueado");
    }else{
        printf(red"Bloqueado");
    }
    printf(cian"\nNombre: %s"reset,aux->nombre);
}