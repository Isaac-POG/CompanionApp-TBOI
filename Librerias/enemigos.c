#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Lista/list.h"
#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

tipoEnemigo * copiarInformacionEnemigo(char * lineaLeida)
{
    tipoEnemigo * nuevoEnemigo = malloc (sizeof(tipoEnemigo));
	char * fragmento;

    //ID Enemigo
	fragmento = strtok(lineaLeida, ";");
	nuevoEnemigo->ID = strtol(fragmento, NULL, 10);

    //Enemigo encontrado
	fragmento = strtok(NULL, ";");
	nuevoEnemigo->encontrado = strtol(fragmento, NULL, 10);

	//Nombre enemigo
	fragmento = strtok(NULL, ";");
    convertirMayuscula(fragmento);
	strcpy(nuevoEnemigo->nombre, fragmento);

    //Vida del enemigo
	fragmento = strtok(NULL, ";");
	nuevoEnemigo->vida = strtol(fragmento, NULL, 10);

    //Ubicacion del Enemigo
	fragmento = strtok(NULL, ";");
    convertirMayuscula(fragmento);
    if(fragmento[0] == '"'){
        //Eliminacion de las comillas iniciales
		memmove(fragmento, fragmento + 1, strlen(fragmento));
		
		//Copia de informacion
		strcpy(nuevoEnemigo->ubicacion[0], fragmento);
	
		short largo; //Almacenar largo de la cadena
		short cantidad = 1; //Contador

		//Reiteracion que finaliza cuando no posea mas tipos
		do
		{
			fragmento = strtok(NULL, ";");
			largo = strlen(fragmento) - 1;

			if(fragmento[largo-1] != '"') //Mientras no reconozca las comillas finales
			{
                convertirMayuscula(fragmento);
				strcpy(nuevoEnemigo->ubicacion[cantidad], fragmento);
			}

			cantidad++;
		} while (fragmento[largo-1] != '"');

		//Eliminacion de la comillas finales
		fragmento[largo] = '\0';
        fragmento[largo-1] = '\0';

        convertirMayuscula(fragmento);
		strcpy(nuevoEnemigo->ubicacion[cantidad - 1], fragmento);
        nuevoEnemigo->cantidadUbicacion = cantidad;
    }else{
        strcpy(nuevoEnemigo->ubicacion[0], fragmento);
        nuevoEnemigo->cantidadUbicacion = 1;
    }
    
	return nuevoEnemigo;
}

void importarArchivoEnemigos(HashMap * mapaEnemigos, List * listaEnemigos){
    FILE * archivo = fopen("Archivos/enemigos.txt", "r");
    if(archivo == NULL)return;

    char lineaLeida[300];

    fgets(lineaLeida, 299, archivo);

    while(fgets(lineaLeida, 299, archivo))
    {
        tipoEnemigo * nuevoEnemigo = copiarInformacionEnemigo(lineaLeida);
        insertMap(mapaEnemigos, &nuevoEnemigo->nombre, nuevoEnemigo);
		pushBack(listaEnemigos, nuevoEnemigo);
    } 

    fclose(archivo);
}

void buscarEnemigoEspecifico(HashMap * mapaEnemigos, char * nombreEnemigo){
    initscr();
    tipoEnemigo * aux = searchMap(mapaEnemigos,nombreEnemigo);
    if(!aux){
        printw("\nEl enemigo que ingreso no existe\n");
        esperarTecla();
        endwin();
        return;
    }
    printw("\nID: %d ",aux->ID);

    if(aux->encontrado){
        printw("Encontrado");
    }else{
        printw("No encontrado");
    }
    printw("\nNombre: %s",aux->nombre);
    printw("\nVida: %d\n",aux->vida);
    if(aux->cantidadUbicacion == 1){
        printw("\nUbicacion: %s\n",aux->ubicacion[0]);
    }else{
        int i;
        printw("\nUbicaciones: ");
        for(i = 0; i < aux->cantidadUbicacion ; i++){
            if(i != 0) printw(", ");
            printw("%s",aux->ubicacion[i]);
        }
        printw("\n");
    }

    esperarTecla();
    endwin();
}

void mostrarEnemigos(HashMap * mapaEnemigos){
    initscr();
    scrollok(stdscr, TRUE);

	for(int i = 1; i < 283; i++)
	{
		tipoEnemigo * aux = firstMap(mapaEnemigos);
		while(aux != NULL)
		{
			if(aux->ID == i)
			{
				wprintw(stdscr,"%s ", aux->nombre);
                if(strlen(aux->nombre) < 22){
                    for(int k = strlen(aux->nombre); k < 22; k++){
                        wprintw(stdscr," ");
                    }
                }
                
				if(aux->encontrado == 0) wprintw(stdscr,"No encontrado \n");
				else wprintw(stdscr,"Encontrado \n" );
			}
			aux = nextMap(mapaEnemigos);
		}
		if(i % (stdscr->_maxy - 1) == 0 || i == 282)
		{
			wrefresh(stdscr);
			esperarTecla();
		}
	}
    endwin();
}

void encontrarEnemigo(HashMap * mapaEnemigos, char * nombreEnemigo){
    initscr();
	tipoEnemigo * enemigoBuscado = searchMap(mapaEnemigos, nombreEnemigo);

	if(enemigoBuscado != NULL)
	{
		if(enemigoBuscado->encontrado == 0)
		{
			printw("\nSe actualizo informacion del item\n");
			printw("%s ENCONTRADO\n", nombreEnemigo);
			enemigoBuscado->encontrado = 1;
		}
		else
		{
			printw("El item %s ya se habia encontrado", nombreEnemigo);
		}
	}
	else
	{
		printw("\nEl item con nombre %s no existe\n", nombreEnemigo);
	}
	esperarTecla();
	endwin();
}

void guardarInfoEnemigos(List * listaEnemigos){
	FILE * archivo = fopen("Archivos/enemigos.txt", "w");
    if(archivo == NULL) return;
    fprintf(archivo,"id;encontrado;nombre;vida;ubicacion\n");
	int flag;

	tipoEnemigo* aux = firstList(listaEnemigos);
	while(aux != NULL){
		fprintf(archivo,"%d;%d;%s;%d;",aux->ID,aux->encontrado,aux->nombre,aux->vida);
		if(aux->cantidadUbicacion == 1){
			fprintf(archivo,"%s",aux->ubicacion[0]);
		}else{
			fprintf(archivo,"\"");
			flag = 0;
			for(int k=0 ; k < aux->cantidadUbicacion ; k++){
				if(flag == 0){
					flag = 1;
				}else{
					fprintf(archivo,";");
				}
				fprintf(archivo,"%s",aux->ubicacion[k]);
			}
			fprintf(archivo,"\"\n");
		}
		aux = nextList(listaEnemigos);
	}
	fclose(archivo);
}