#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Lista/list.h"
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

void importarArchivoLogros(HashMap * mapaLogros, List * listaLogros){
    FILE * archivo = fopen("Archivos/logros.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[200];

    fgets(lineaLeida, 199, archivo);

    while(fgets(lineaLeida, 199, archivo))
    {
        tipoLogro * nuevoLogro = copiarInformacionLogro(lineaLeida);
        insertMap(mapaLogros, &nuevoLogro->ID, nuevoLogro);
        pushBack(listaLogros, nuevoLogro);
    } 

    fclose(archivo);
}

void mostrarLogros(HashMap * mapaLogros)
{
    initscr();

    scrollok(stdscr, TRUE);

    tipoLogro * aux = firstMap(mapaLogros);
    
    for(int j=1; j<404 ; j++){
        aux = firstMap(mapaLogros);
        while(aux){
            if(j == aux->ID){
                wprintw(stdscr,"ID: %d ",aux->ID);
                if(aux->ID < 10) wprintw(stdscr,"  ");
                else if(aux->ID < 100) wprintw(stdscr," ");

                if(aux->desbloqueado){
                    wprintw(stdscr,"Desbloqueado");
                }else{
                    wprintw(stdscr,"Bloqueado   ");
                }
                wprintw(stdscr," -> %s\n",aux->nombre);
                break;
            }
            
            aux = nextMap(mapaLogros);
        }
        if(j % (stdscr->_maxy - 1) == 0 || j == 403) 
        {
            wrefresh(stdscr);
            esperarTecla();
            wrefresh(stdscr);
        }
    }
    endwin();
}

void buscarLogroEspecifico(HashMap * mapaLogros, int id)
{
    initscr();
    tipoLogro * aux = searchMap(mapaLogros,&id);
    if(!aux){
        printw("\nEl logro que ingreso no existe\n");
        getch();
        return;
    }
    printw("\nID: %d ",aux->ID);

    if(aux->desbloqueado){
        printw("Desbloqueado");
    }else{
        printw("Bloqueado");
    }
    printw("\nNombre: %s",aux->nombre);
    printw("\nDescripcion: %s\n",aux->descripcion);
    printw("Como conseguir:\n%s",aux->comoConseguir);
    esperarTecla();
    endwin();
}

void desbloquearLogro(HashMap * mapaLogros, int id){
    initscr();
    tipoLogro * aux = searchMap(mapaLogros,&id);
    
    if(!aux){
        printw("\nEl logro que ingreso no existe\n");
        esperarTecla();
        return;
    }
    if(aux->desbloqueado){
        printw("\nEl logro ya estaba desbloqueado de antes\n");
        esperarTecla();
        return;
    }
    aux->desbloqueado = 1;

    printw("\nID: %d ",aux->ID);

    if(aux->desbloqueado){
        printw("Desbloqueado");
    }else{
        printw("Bloqueado");
    }
    printw("\nNombre: %s",aux->nombre);
    esperarTecla();
    endwin();
}

void guardarInfoLogros(List * listaLogros){
	FILE * archivo = fopen("Archivos/logros.txt", "w");
    if(archivo == NULL) return;
    fprintf(archivo,"ID,Desbloqueado,Nombre de Logros,Descripcion,Como conseguirlo\n");

    tipoLogro * aux = firstList(listaLogros);
    while(aux != NULL){
        fprintf(archivo,"%d;%d;%s;%s;%s",aux->ID,aux->desbloqueado,aux->nombre,aux->descripcion,aux->comoConseguir);
        aux = nextList(listaLogros);
    }
	
	fclose(archivo);
}