#include <stdlib.h>
#include <ncurses.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/personajes.h"
#include "Librerias/logros.h"
#include "Librerias/items.h"

/* PROTOTIPADO DE FUNCIONES */

//Funcion que delimita el tamaño del menu a usar
WINDOW * crearVentana(int cantOpciones);

//Funcion para mostrar el subMenu de funciones
void mostrarSubMenu(HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems);

//Funcion que inicia las funciones principales del programa
void funcionesOpcion(int opcion,HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems);

//Funcion para mostrar el menu principal
void mostrarMenu(HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems);


int main()
{
	pantallaInicial();

	//Iniciar la libreria ncurses.h
	initscr();
	
	//Creacion de Mapas
	HashMap * mapaPersonajes = createMap(10);
	HashMap * mapaLogros = createMap(576);
	HashMap * mapaItems = createMap(600);
	
	//Importacion de Informacion
	importarArchivoItems(mapaItems);
	importarArchivoLogros(mapaLogros);
	importarArchivoPersonajes(mapaPersonajes);
	
	mostrarMenu(mapaPersonajes, mapaLogros, mapaItems);

	//Libera memoria
	free(mapaPersonajes);
	free(mapaLogros);
	free(mapaItems);
	
	clear();
	printw("FIN DEL PROGRAMA\n");
	getch();

	//Fin de la libreria ncurses.h
	endwin();
	return 0;
}

WINDOW * crearVentana(int cantOpciones)
{
	int xMax, yMax;
	clear();

    getmaxyx(stdscr, yMax, xMax);
    WINDOW * ventana = newwin(cantOpciones, 50, yMax/2, 50);

    //Hacer que funcionen las teclas de flecha
    keypad(ventana, TRUE);

	return ventana;
}


void mostrarMenu(HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems)
{
    char opciones[10][40] = {"Menu de Desbloqueo","Guardar Informacion","Buscar un Item Especifico","Buscar un Logro Especifico","Buscar un Enemigo Especifico","Mostrar Todos los Personajes","Mostrar Todos los Items","Mostrar Todos los Logros","Mostrar Todos los Enemigos","Salir del Programa"};
    int eleccion = -1, iluminar = 0;

    WINDOW * ventana = crearVentana(12);
    
    while(eleccion)
    {
        //Delimitar el menu
        box(ventana, 0, 0);
        refresh();

        //Muestra las opciones
        for(int i = 0; i < 10; i++)
        {
        	//Si es la posicion actual, la ilumina
            if(i == iluminar) wattron(ventana, A_REVERSE);

            //Si no, la muestra como texto plano
            mvwprintw(ventana, i+1, 1, opciones[i]);
            wattroff(ventana, A_REVERSE);
        }

        //El usuario ingresa alguna entrada desde el teclado (sin necesidad de usar enter)
        eleccion = wgetch(ventana);

        switch (eleccion) //Dependiendo de la elección
        {
        case KEY_UP:
            iluminar--;
            if(iluminar == -1) iluminar = 9; //Si llega hasta arriba, mueve el cursor al final del menu
            break;
        
        case KEY_DOWN:
            iluminar++;
            if(iluminar == 10) iluminar = 0; //Si llega al final mueve el cursor al inicio del menu
            break;
        }

        if(eleccion == 10) //Si se ingreso un enter, significa que se usara una funcion
        {   
            if(iluminar == 9) break; //Posicion donde esta el fin del programa
            funcionesOpcion(iluminar,mapaPersonajes, mapaLogros, mapaItems); //Accede a la funcion presentada
        }

        wrefresh(ventana); //Recarga la ventana
    }
}

void mostrarSubMenu(HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems)
{
	char opciones[6][50] = {"Desbloquear Personaje","Avance de Marcas de Logros","Encontrar Item","Desbloquear Logro","Encontrar Enemigo","Salir del Menu"};
    int eleccion = -1, iluminar = 0;

    WINDOW * ventana = crearVentana(8);
    
    while(eleccion)
    {
        //Delimitar el menu
        box(ventana, 0, 0);
        refresh();
    
        for(int i = 0; i < 6; i++)
        {
            if(i == iluminar) wattron(ventana, A_REVERSE);
            mvwprintw(ventana, i+1, 1, opciones[i]);
            wattroff(ventana, A_REVERSE);
        }

        eleccion = wgetch(ventana);
        switch (eleccion)
        {
        case KEY_UP:
            iluminar--;
            if(iluminar == -1) iluminar = 5;
            break;
        
        case KEY_DOWN:
            iluminar++;
            if(iluminar == 6) iluminar = 0;
            break;

        default:
            break;
        }

        if(eleccion == 10) //Muestra el enter
        {   
            if(iluminar == 5) break;
            funcionesOpcion(iluminar + 9, mapaPersonajes, mapaLogros, mapaItems); 
        }
        wrefresh(ventana);
    }
}

void funcionesOpcion(int opcion,HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems)
{
	char nombreBuscado[40];
	int idBuscado;


	clear();
	echo();

	switch (opcion)
	{
	case 0:
		mostrarSubMenu(mapaPersonajes, mapaLogros, mapaItems);
		break;
	case 1:
		break;
		
	case 2:
		printw("\nIngrese el nombre del item: ");
		scanw("%39[^\n]s", nombreBuscado);
		convertirMayuscula(nombreBuscado);
		buscarItemEspecifico(mapaItems, nombreBuscado);
		break;
	case 3:
		printw("\nIngrese el nombre del item: ");
		scanw("%i", &idBuscado);
		buscarLogroEspecifico(mapaLogros, idBuscado);
		break;
	case 4:
		/* code */
		break;
	case 5:
		mostrarPersonajes(mapaPersonajes);
		break;
	case 6:
		mostrarTodosItems(mapaItems);
		break;
	case 7:
		mostrarLogros(mapaLogros);
		break;
	case 8:
		/* code */
		break;
	case 9:
		break;
	case 10:
		break;
	case 11:
		break;
	case 12:
		break;
	case 13:
		break;
	}

	clear();
	noecho();
}