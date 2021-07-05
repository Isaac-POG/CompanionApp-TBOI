#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Lista/list.h"
#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

//Calcula el porcentaje avanzado en enemigos
float calculoTotalEnemigos(List * listaEnemigos)
{
	float totalEnemigos = 0;
	tipoEnemigo * aux = firstList(listaEnemigos);

	while(aux != NULL)
	{
		if(aux->encontrado == 1) totalEnemigos++;
		aux = nextList(listaEnemigos);
	}

	return totalEnemigos;
}

//Copia la informacion de la lineaLeida a un tipoEnemigo
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

	//Caso en que tenga mas de 2 ubicaciones
    if(fragmento[0] == '"'){
        //Eliminacion de las comillas iniciales
		memmove(fragmento, fragmento + 1, strlen(fragmento));
		
		//Copia de informacion
		strcpy(nuevoEnemigo->ubicacion[0], fragmento);
	
		short largo; //Almacenar largo de la cadena
		short cantidad = 1; //Contador

		//Reiteracion que finaliza cuando no posea mas ubicaciones
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
		//Caso en que solo tenga 1 ubicacion
        strcpy(nuevoEnemigo->ubicacion[0], fragmento);
        nuevoEnemigo->cantidadUbicacion = 1;
    }
    
	return nuevoEnemigo;
}

//Importa el archivo enemigos
void importarArchivoEnemigos(HashMap * mapaEnemigos, List * listaEnemigos){
	//Se busca el archivo
    FILE * archivo = fopen("Archivos/enemigos.txt", "r");
    if(archivo == NULL)return;

    char lineaLeida[300];

	//Se ignora la primera linea
    fgets(lineaLeida, 299, archivo);

	//Se copia la informacion
    while(fgets(lineaLeida, 299, archivo))
    {
        tipoEnemigo * nuevoEnemigo = copiarInformacionEnemigo(lineaLeida);
        insertMap(mapaEnemigos, &nuevoEnemigo->nombre, nuevoEnemigo);
		pushBack(listaEnemigos, nuevoEnemigo);
    } 
	//Se cierra el archivo
    fclose(archivo);
}

//Busca un enemigo ingresado por el usuario
void buscarEnemigoEspecifico(HashMap * mapaEnemigos, char * nombreEnemigo){
    //Inicio ncurses
	initscr();

    tipoEnemigo * aux = searchMap(mapaEnemigos,nombreEnemigo);
	attron(A_BOLD);

    if(!aux){
		//Caso de enemigo no encontrado
		attron(COLOR_PAIR(3));
        printw("\nEl enemigo que ingreso no existe\n");
		attroff(COLOR_PAIR(3));
        esperarTecla(0);
        endwin();
        return;
    }

	attron(COLOR_PAIR(4));
	printw("\nNombre: %s ",aux->nombre);
	attroff(COLOR_PAIR(4));

	//Diferenciar si fue encontrado el enemigo
    if(aux->encontrado){
		attron(COLOR_PAIR(2));
        printw("Encontrado");
		attroff(COLOR_PAIR(2));
    }else{
		attron(COLOR_PAIR(3));
        printw("No encontrado");
		attroff(COLOR_PAIR(3));
    }

	//Mostrar sus datos
	printw("\nID: %d ",aux->ID);
    printw("\nVida: %d",aux->vida);

	//Mostrar su(s) ubicacion(es)
    if(aux->cantidadUbicacion == 1){
        printw("\nUbicacion: %s\n",aux->ubicacion[0]);
    }else{
        printw("\nUbicaciones: ");

        for(int i = 0; i < aux->cantidadUbicacion ; i++){
            if(i != 0) printw(", ");
            printw("%s",aux->ubicacion[i]);
        }
        printw("\n");
    }

	attroff(A_BOLD);
    esperarTecla(0);
    endwin();
}

//Muestra todos los enemigos
void mostrarEnemigos(List * listaEnemigos){
    //Inicio del Ncurses.h
	initscr();

	//Activar el scroll
    scrollok(stdscr, TRUE);

	tipoEnemigo * aux = firstList(listaEnemigos);
	int i = 1;

	//Se recorre toda la lista
	while(aux != NULL)
	{
		attron(A_BOLD);
		attron(COLOR_PAIR(4));

		wprintw(stdscr,"%s ", aux->nombre);

		attroff(COLOR_PAIR(4));
		
		//Coloca espacios faltantes para que se muestre ordenados.
		if(strlen(aux->nombre) < 22){
			for(int k = strlen(aux->nombre); k < 22; k++) wprintw(stdscr," ");
		}
		
		if(aux->encontrado == 0){
			//Caso en el que no se ha encontrado el enemigo
			attron(COLOR_PAIR(3));
			wprintw(stdscr,"No encontrado \n");
			attroff(COLOR_PAIR(3));
		}
		else{
			//Caso en el que se ha encontrado el enemigo
			attron(COLOR_PAIR(2));
			wprintw(stdscr,"Encontrado \n" );
			attroff(COLOR_PAIR(2));
		}

		//Pausa para poder ver los enemigos, la cantidad depende del tamaño de la terminal
		if(i % (stdscr->_maxy - 1) == 0 || i == 282)
		{
			wrefresh(stdscr);
			esperarTecla(0);
		}
		
		aux = nextList(listaEnemigos);
		i++;
	}
	
	attroff(A_BOLD);
    endwin();
}

//El usuario indica si encontro el enemigo
void encontrarEnemigo(HashMap * mapaEnemigos, char * nombreEnemigo){
    initscr();
	
	//Se busca el nombre del enemigo en el mapa
	tipoEnemigo * enemigoBuscado = searchMap(mapaEnemigos, nombreEnemigo);

	if(enemigoBuscado != NULL)
	{
		if(enemigoBuscado->encontrado == 0)
		{
			//Caso en que no se ha encontrado
			printw("\nSe actualizo informacion del item\n");
			attron(COLOR_PAIR(2));
			printw("%s ENCONTRADO\n", nombreEnemigo);
			attroff(COLOR_PAIR(2));
			enemigoBuscado->encontrado = 1; //Cambiar a si se encuentra
		}
		else
		{
			//Caso en el cual ya se habia encontrado anteriormente
			printw("El item %s ya se habia encontrado", nombreEnemigo);
		}
	}
	else
	{
		//Caso en que el nombre no exista
		printw("\nEl item con nombre %s no existe\n", nombreEnemigo);
	}

	esperarTecla(0);
	endwin();
}

//Se guarda toda la informacion de los enemigos
void guardarInfoEnemigos(List * listaEnemigos){
	//Abrir el archivo de enemigos, en este archivo se guardaran los datos actualizados
	FILE * archivo = fopen("Archivos/enemigos.txt", "w");
    if(archivo == NULL) return;

	//Se guarda la primera linea
    fprintf(archivo,"id;encontrado;nombre;vida;ubicacion\n");
	int flag; //Servira para imprimir los punto coma (;) en el caso de que tenga 2 o mas ubicaciones

	tipoEnemigo* aux = firstList(listaEnemigos);
	while(aux != NULL){
		//Se guardan los datos
		fprintf(archivo,"%d;%d;%s;%d;",aux->ID,aux->encontrado,aux->nombre,aux->vida);

		//Diferenciar en si tiene 1 ubicacion o mas
		if(aux->cantidadUbicacion == 1){
			fprintf(archivo,"%s",aux->ubicacion[0]); //Se guarda la unica ubicacion
		}else{
			fprintf(archivo,"\""); //Se guarda la doble comilla inicial
			flag = 0; //Inicia en 0, para que el punto coma (;) no se guarde la primera vez
			
			for(int k=0 ; k < aux->cantidadUbicacion ; k++){

				if(flag == 0) flag = 1;
				else fprintf(archivo,";");

				fprintf(archivo,"%s",aux->ubicacion[k]); //Se guardan las ubicaciones
			}
			fprintf(archivo,"\"\n"); //Se guarda la doble comilla final
		}
		aux = nextList(listaEnemigos);
	}
	fclose(archivo);
}