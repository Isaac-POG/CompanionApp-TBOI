#include <stdlib.h>
#include <ncurses.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/TDA_Lista/list.h"
#include "Librerias/personajes.h"
#include "Librerias/logros.h"
#include "Librerias/items.h"
#include "Librerias/enemigos.h"

/* PROTOTIPADO DE FUNCIONES */

//Funcion para mostrar el menu principal
int mostrarMenu();

//Funcion para mostrar el subMenu de funciones
int mostrarSubMenu();

//Funcion que inicia las funciones principales del programa
void funcionesOpcion(int opcion,HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems, HashMap * mapaEnemigos, List *, List *, List *, List *);

int main()
{	
	//Iniciar la libreria ncurses.h
	initscr();

	//Activa los colores
	iniciarColores();

	//Muestra la pantalla de inicio
	pantallaInicial();

	//Creacion de TDAs
	HashMap * mapaPersonajes = createMap(10);
	HashMap * mapaLogros = createMap(576);
	HashMap * mapaItems = createMap(600);
	HashMap * mapaEnemigos = createMap(410);
	List * listaPersonajes = createList();
	List * listaItems = createList();
	List * listaLogros = createList();
	List * listaEnemigos = createList();
	
	//Importacion de Informacion
	importarArchivoPersonajes(mapaPersonajes, listaPersonajes);
	importarArchivoItems(mapaItems, listaItems);
	importarArchivoLogros(mapaLogros, listaLogros);
	importarArchivoEnemigos(mapaEnemigos, listaEnemigos);

	int eleccionUsuario = 0;
	
	//Forma de repetir el menu
	while(eleccionUsuario != 9)
	{
		eleccionUsuario = mostrarMenu();
		if(eleccionUsuario == 9) break;

		funcionesOpcion(eleccionUsuario, mapaPersonajes, mapaLogros, mapaItems, mapaEnemigos, listaPersonajes, listaItems, listaLogros, listaEnemigos);
		
		if(eleccionUsuario == 0)
		{
			int eleccionSubMenu = 0;
			while(eleccionSubMenu != 14)
			{
				eleccionSubMenu = mostrarSubMenu();
				if(eleccionSubMenu == 14) break;
		
				funcionesOpcion(eleccionSubMenu, mapaPersonajes, mapaLogros, mapaItems, mapaEnemigos, listaPersonajes, listaItems, listaLogros, listaEnemigos);
			}
		}
	}

	//Liberar memoria de TDAs
	free(mapaPersonajes);
	free(mapaLogros);
	free(mapaItems);
	free(mapaEnemigos);
	free(listaPersonajes);
	free(listaItems);
	free(listaLogros);
	free(listaEnemigos);

	pantallaFinal();

	//Fin de la libreria ncurses.h
	endwin();
	return 0;
}

int mostrarMenu()
{
    char opciones[10][40] = {"Menu de Desbloqueo","Guardar Informacion","Buscar un Item Especifico","Buscar un Logro Especifico","Buscar un Enemigo Especifico","Mostrar Todos los Personajes","Mostrar Todos los Items","Mostrar Todos los Logros","Mostrar Todos los Enemigos","Salir del Programa"};
    int eleccion = -1, iluminar = 0;

    WINDOW * ventana;
    
    while(eleccion)
    {
    	//Establecer el tamaño del menu
    	ventana = crearVentana(12);
        
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
		case 10: //Si se apreta la tecla ENTER, significa que se quiere usar una opcion del menu
			return iluminar;
            break;
        }

    }
}

int mostrarSubMenu()
{
	//Opciones dentro de este submenú
	char opciones[6][50] = {"Desbloquear Personaje","Avance de Marcas de Logros","Encontrar Item","Desbloquear Logro","Encontrar Enemigo","Salir del Menu"};
    int eleccion = -1, iluminar = 0;

	//Creación de ventana del menú
    WINDOW * ventana;
    
    while(eleccion)
    {
    	//Establecer el tamaño del menu
    	ventana = crearVentana(8);

        //Delimitar la caja del menu
        box(ventana, 0, 0);
        refresh();

        for(int i = 0; i < 6; i++)
        {
            if(i == iluminar) wattron(ventana, A_REVERSE);
            mvwprintw(ventana, i+1, 1, opciones[i]);
            wattroff(ventana, A_REVERSE);
        }

		//Ingreso de las teclas
        eleccion = wgetch(ventana);
        switch (eleccion)
        {
        case KEY_UP:
            iluminar--;
            if(iluminar == -1) iluminar = 5; //Si llega hasta arriba, mueve el cursor al final del menu
            break;
        case KEY_DOWN:
            iluminar++;
            if(iluminar == 6) iluminar = 0; //Si llega al final mueve el cursor al inicio del menu
            break;
 		case 10: //Si se apreta la tecla ENTER, significa que se quiere usar una opcion del menu
            return iluminar + 9;
        }
        wrefresh(ventana);
    }
}

void funcionesOpcion(int opcion,HashMap * mapaPersonajes, HashMap * mapaLogros, HashMap * mapaItems, HashMap * mapaEnemigos, List * listaPersonajes, List * listaItems, List * listaLogros, List * listaEnemigos)
{
	char nombreBuscado[40];
	int idBuscado;

	clear();
	echo();

	switch (opcion)
	{
	case 1:
		guardarInfoPersonajes(listaPersonajes);
		guardarInfoItems(listaItems);
		guardarInfoEnemigos(listaEnemigos);
		guardarInfoLogros(listaLogros);
		clear();
		printw("Se guardaron los cambios\nApriete cualquier tecla para avanzar");
		getch();
		break;	
	case 2:
		printw("Ingrese el nombre del item: ");
		scanw("%39[^\n]s", nombreBuscado);
		convertirMayuscula(nombreBuscado);
		buscarItemEspecifico(mapaItems, nombreBuscado);
		break;
	case 3:
		printw("Ingrese el ID del logro: ");
		scanw("%i", &idBuscado);
		buscarLogroEspecifico(mapaLogros, idBuscado);
		break;
	case 4:
		printw("Ingrese el nombre del enemigo: ");
		scanw("%39[^\n]s", nombreBuscado);
		convertirMayuscula(nombreBuscado);
		buscarEnemigoEspecifico(mapaEnemigos, nombreBuscado);
		break;
	case 5:
		mostrarPersonajes(mapaPersonajes, listaPersonajes);
		break;
	case 6:
		mostrarTodosItems(listaItems);
		break;
	case 7:
		mostrarLogros(mapaLogros);
		break;
	case 8:
		mostrarEnemigos(mapaEnemigos);
		break;
	case 9:
		desbloquearPersonajes(listaPersonajes,mapaPersonajes);
		break;
	case 10:
		avanceMarcasLogros(listaPersonajes,mapaPersonajes);
		break;
	case 11:
		printw("Ingrese el nombre del Item que encontro: ");
		scanw("%39[^\n]s", nombreBuscado);
		convertirMayuscula(nombreBuscado);
		encontrarItem(mapaItems, nombreBuscado);
		break;
	case 12:
		printw("Ingrese el ID del logro que desbloqueo: ");
		scanw("%i", &idBuscado);
		desbloquearLogro(mapaLogros, idBuscado);
		break;
	case 13:
		printw("Ingrese el nombre del enemigo que encontro: ");
		scanw("%39[^\n]s", nombreBuscado);
		convertirMayuscula(nombreBuscado);
		encontrarEnemigo(mapaEnemigos,nombreBuscado);
		break;
	}

	clear();
	noecho();
}