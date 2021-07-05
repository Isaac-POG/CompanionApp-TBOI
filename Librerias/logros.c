#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Lista/list.h"
#include "TDA_Mapa/hashmap.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

//Calcula el porcentaje avanzado en los logros
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

//Copia la informacion de la lineaLeida a un tipoLogro
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

//Importa el archivo logros
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

//Muestra todos los logros
void mostrarLogros(List * listaLogros)
{
    //Inicio de la ncureses
    initscr();
    //Activar el scroll
    scrollok(stdscr, TRUE);

    tipoLogro * aux = firstList(listaLogros);
    int j = 1; //Variable para mostrar por pantalla los logros que quepan en la ventana

    while(aux != NULL){
        attron(A_BOLD); //Letra en negrita
        attron(COLOR_PAIR(4)); //Letra en color

        wprintw(stdscr,"ID: %d ",aux->ID);
        
        attroff(COLOR_PAIR(4)); //Desactivar la letra en color

        //Sirve para mostrar los logros ordenados
        if(aux->ID < 10) wprintw(stdscr,"  ");
        else if(aux->ID < 100) wprintw(stdscr," ");

        //Diferenciar si el logro se encuentra desbloqueado o no
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

        //Funciona para mostrar por pantalla los logros que quepan en la ventana de la terminal.
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

//Busca un logro ingresado por el usuario
void buscarLogroEspecifico(HashMap * mapaLogros, int id)
{
    //inicio de la ncurses
    initscr();

    tipoLogro * aux = searchMap(mapaLogros,&id);
    attron(A_BOLD); //Letras en negrita

    if(!aux){
        attron(COLOR_PAIR(3)); //Letras en color
        printw("\nEl logro que ingreso no existe\n");
        attroff(COLOR_PAIR(3)); //Desactivar letras en color
        attroff(A_BOLD); //Desactivar letras en negrita
        esperarTecla(0); //"Presione una tecla para avanzar"
        return;
    }

    attron(COLOR_PAIR(4));
    printw("\nID: %d ",aux->ID);
    attroff(COLOR_PAIR(4));

    //Diferenciar si el logro esta desbloqueado o no
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

//El usuario indica si desbloqueo el logro
void desbloquearLogro(HashMap * mapaLogros, int id)
{
    //Inicio de la ncureses
    initscr();
    tipoLogro * aux = searchMap(mapaLogros,&id);
    
    //Verificar si el logro existe
    if(!aux){
        printw("\nEl logro que ingreso no existe\n");
        esperarTecla(0);
        return;
    }

    //Verificar si el logro se encuentra desbloqueado
    if(aux->desbloqueado){
        printw("\nEl logro ya estaba desbloqueado de antes\n");
        esperarTecla(0);
        return;
    }
    aux->desbloqueado = 1;

    printw("\nID: %d ",aux->ID);

    attron(COLOR_PAIR(2));
    printw("Desbloqueado");
    attroff(COLOR_PAIR(2));

    printw("\nNombre: %s\n",aux->nombre);
    esperarTecla(0);
    endwin();
}

//Se guarda toda la informacion de los logros
void guardarInfoLogros(List * listaLogros)
{
    //Se busca el archivo logros.txt para escribir los datos actualizados
	FILE * archivo = fopen("Archivos/logros.txt", "w");
    if(archivo == NULL) return;

    //La primera linea
    fprintf(archivo,"ID,Desbloqueado,Nombre de Logros,Descripcion,Como conseguirlo\n");

    //Se guarda toda la informacion de los logros
    tipoLogro * aux = firstList(listaLogros);
    while(aux != NULL){
        fprintf(archivo,"%d;%d;%s;%s;%s",aux->ID,aux->desbloqueado,aux->nombre,aux->descripcion,aux->comoConseguir);
        aux = nextList(listaLogros);
    }
	
	fclose(archivo);
}