#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Lista/list.h"
#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

float calculoTotalLogros(List * listaLogros)
{
    tipoLogro * aux = firstList(listaLogros);
    float totalLogros = 0;
    
    while (aux != NULL)
    {
        if(aux->desbloqueado == 1) totalLogros++;
        aux = nextList(listaLogros);
    }
    
    return totalLogros;
}

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
    //Se busca el archivo
    FILE * archivo = fopen("Archivos/logros.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[200];

    //Se ignora la primera linea
    fgets(lineaLeida, 199, archivo);

    //Se copia la informacion
    while(fgets(lineaLeida, 199, archivo))
    {
        tipoLogro * nuevoLogro = copiarInformacionLogro(lineaLeida);
        insertMap(mapaLogros, &nuevoLogro->ID, nuevoLogro);
        pushBack(listaLogros, nuevoLogro);
    }
    //Se cierra el archivo
    fclose(archivo);
}

void mostrarLogros(List * listaLogros)
{
    initscr();

    scrollok(stdscr, TRUE);

    tipoLogro * aux = firstList(listaLogros);
    int j = 1;

    while(aux != NULL){
        attron(A_BOLD);
        attron(COLOR_PAIR(4));

        wprintw(stdscr,"ID: %d ",aux->ID);
        
        attroff(COLOR_PAIR(4));

        if(aux->ID < 10) wprintw(stdscr,"  ");
        else if(aux->ID < 100) wprintw(stdscr," ");

        if(aux->desbloqueado){
            attron(COLOR_PAIR(2));
            wprintw(stdscr,"Desbloqueado");
            attroff(COLOR_PAIR(2));
        }else{
            attron(COLOR_PAIR(3));
            wprintw(stdscr,"Bloqueado   ");
            attroff(COLOR_PAIR(3));
        }
        wprintw(stdscr," -> %s\n",aux->nombre);

        if(j % (stdscr->_maxy - 1) == 0 || j == 403) 
        {
            wrefresh(stdscr);
            esperarTecla(0);
        }
        
        aux = nextList(listaLogros);
        j++;
    }
    
    attroff(A_BOLD);
    endwin();
}

void buscarLogroEspecifico(HashMap * mapaLogros, int id)
{
    initscr();
    tipoLogro * aux = searchMap(mapaLogros,&id);
    attron(A_BOLD);

    if(!aux){
        attron(COLOR_PAIR(3));
        printw("\nEl logro que ingreso no existe\n");
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
        getch();
        return;
    }

    attron(COLOR_PAIR(4));
    printw("\nID: %d ",aux->ID);
    attroff(COLOR_PAIR(4));

    if(aux->desbloqueado){
        attron(COLOR_PAIR(2));
        printw("Desbloqueado");
        attroff(COLOR_PAIR(2));
    }else{
        attron(COLOR_PAIR(3));
        printw("Bloqueado");
        attroff(COLOR_PAIR(3));
    }
    printw("\nNombre: %s",aux->nombre);
    printw("\nDescripcion: %s\n",aux->descripcion);

    attron(COLOR_PAIR(2));
    printw("Como conseguir:\n%s",aux->comoConseguir);
    attron(COLOR_PAIR(2));

    attroff(A_BOLD);
    esperarTecla(0);
    endwin();
}

void desbloquearLogro(HashMap * mapaLogros, int id){
    initscr();
    tipoLogro * aux = searchMap(mapaLogros,&id);
    
    if(!aux){
        printw("\nEl logro que ingreso no existe\n");
        esperarTecla(0);
        return;
    }
    if(aux->desbloqueado){
        printw("\nEl logro ya estaba desbloqueado de antes\n");
        esperarTecla(0);
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
    esperarTecla(0);
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