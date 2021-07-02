#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Mapa/hashmap.h"
#include "TDA_Lista/list.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

//Se separa la informacion del personaje y se almacena en un tipoPersonaje
tipoPersonaje * copiarInformacionPersonaje(char * lineaLeida)
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

//Se importa el archivo personajes.txt y almacena su informacion en un mapa
void importarArchivoPersonajes(HashMap * mapaPersonajes, List * listaPersonajes)
{
    FILE * archivo = fopen("Archivos/personajes.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[100];

    fgets(lineaLeida, 99, archivo);

    while(fgets(lineaLeida, 99, archivo))
    {
        tipoPersonaje * nuevoPersonaje = copiarInformacionPersonaje(lineaLeida);
        insertMap(mapaPersonajes, nuevoPersonaje->nombre, nuevoPersonaje);
        pushBack(listaPersonajes, nuevoPersonaje);
    } 

    fclose(archivo);
}

//Se guardan los cambios realizados durante la ejecucion del programa, y estos cambios se guardan en personajes.txt
void guardarInfoPersonajes(List * listaPersonajes)
{
    FILE * archivo = fopen("Archivos/personajes.txt", "w");
    if(archivo == NULL) return;
    
    fprintf(archivo,"ID,Personaje,Desbloqueado,Marcas\n");
    tipoPersonaje * aux = firstList(listaPersonajes);

    while(aux != NULL)
    {
        fprintf(archivo, "%i,%s,%i", aux->ID, aux->nombre, aux->desbloqueado);
        for(int j = 0; j < 10; j++)
        {
            fprintf(archivo, ",%i", aux->marcas[j]);
        }
        fprintf(archivo,"\n");
        aux = nextList(listaPersonajes);
    }
    fclose(archivo);
}

//Funcion que muestra a todos los personajes, ordenados de la misma forma que en el juego
void mostrarPersonajes(HashMap * mapaPersonajes, List * listaPersonajes)
{
    initscr();
    char personajes[16][25];
    int j = 0, eleccion = -1, iluminar = 0;

    tipoPersonaje * auxRecorrido = firstList(listaPersonajes);
    
    //Se almacena en un arreglo de cadenas, todos los personajes que no se hayan desbloqueado
    while(auxRecorrido != NULL)
    {
        strcpy(personajes[j], auxRecorrido->nombre);
        j++;
        auxRecorrido = nextList(listaPersonajes);
    }

    strcpy(personajes[j], "Salir del Menu");
    j++;

    WINDOW * ventana;
    
    while(eleccion)
    {
        //Establecer el tamaño del menu
        ventana = crearVentana(j + 2);
        
        //Delimitar el menu
        box(ventana, 0, 0);
        refresh();
        
        //Muestra los posibles personajes
        for(int i = 0; i < j; i++)
        {
            //Si es la posicion actual, la ilumina
            if(i == iluminar) wattron(ventana, A_REVERSE);

            //Si no, la muestra como texto plano
            mvwprintw(ventana, i+1, 1, personajes[i]);
            wattroff(ventana, A_REVERSE);
        }

        //El usuario ingresa alguna entrada desde el teclado (sin necesidad de usar enter)
        eleccion = wgetch(ventana);

        switch (eleccion) //Dependiendo de la elección
        {
        case KEY_UP:
            iluminar--;
            if(iluminar == -1) iluminar = j - 1; //Si llega hasta arriba, mueve el cursor al final del menu
            break;
        case KEY_DOWN:
            iluminar++;
            if(iluminar == j) iluminar = 0; //Si llega al final mueve el cursor al inicio del menu
            break;
        case 10: //Si se apreta la tecla ENTER, significa que se quiere usar una opcion del menu
            if(iluminar != j - 1)
            {
                tipoPersonaje * nuevoPersonaje = searchMap(mapaPersonajes, personajes[iluminar]);
                clear();
                mostrarPersonaje(nuevoPersonaje->nombre, nuevoPersonaje->desbloqueado, nuevoPersonaje->marcas, 1);
                esperarTecla();
            }
            else return;
            break;
        }

    }
    endwin();
}

//Función que actualiza la informacion de desbloqueo de un personaje
void desbloquearPersonajes(HashMap * mapaPersonajes, List * listaPersonajes)
{
    initscr();

    char personajes[16][25];
    int j = 0, eleccion = -1, iluminar = 0;

    tipoPersonaje * auxRecorrido = firstList(listaPersonajes);
    
    //Se almacena en un arreglo de cadenas, todos los personajes que no se hayan desbloqueado
    while(auxRecorrido != NULL)
    {
        if(auxRecorrido->desbloqueado == 0) 
        {
            strcpy(personajes[j], auxRecorrido->nombre);
            j++;
        }
        auxRecorrido = nextList(listaPersonajes);
    }

    //Si estan todos desbloqueados
    if(j == 0)
    {
        printw("Ha desbloqueado todos los personajes\n");
        esperarTecla();
        endwin();
        return;
    }

    strcpy(personajes[j], "Salir del Menu");
    j++;

    WINDOW * ventana;
    
    while(eleccion)
    {
        //Establecer el tamaño del menu
        ventana = crearVentana(j + 2);
        
        //Delimitar el menu
        box(ventana, 0, 0);
        refresh();
        
        //Muestra los posibles personajes
        for(int i = 0; i < j; i++)
        {
            //Si es la posicion actual, la ilumina
            if(i == iluminar) wattron(ventana, A_REVERSE);

            //Si no, la muestra como texto plano
            mvwprintw(ventana, i+1, 1, personajes[i]);
            wattroff(ventana, A_REVERSE);
        }

        //El usuario ingresa alguna entrada desde el teclado (sin necesidad de usar enter)
        eleccion = wgetch(ventana);

        switch (eleccion) //Dependiendo de la elección
        {
        case KEY_UP:
            iluminar--;
            if(iluminar == -1) iluminar = j - 1; //Si llega hasta arriba, mueve el cursor al final del menu
            break;
        case KEY_DOWN:
            iluminar++;
            if(iluminar == j) iluminar = 0; //Si llega al final mueve el cursor al inicio del menu
            break;
        case 10: //Si se apreta la tecla ENTER, significa que se quiere usar una opcion del menu
            if(iluminar != j - 1)
            {
                tipoPersonaje * nuevoPersonaje = searchMap(mapaPersonajes, personajes[iluminar]);
                nuevoPersonaje->desbloqueado = 1;
                clear();
                printw("%s ahora se encuentra DESBLOQUEADO", nuevoPersonaje->nombre);
                esperarTecla();
                endwin();
            }
            return;
        }

    }
}

//Funcion para actualizar la informacion respecto al avance en las marcas de logro de un personaje
void avanceMarcasLogros(HashMap * mapaPersonajes, char * nombrePersonaje)
{
    initscr();
    tipoPersonaje * aux = searchMap(mapaPersonajes, nombrePersonaje);
    int  opcion;
    char respuesta[20];

    if(aux != NULL)
    {
        if(aux->desbloqueado == 1)
        {
            mostrarPersonaje(aux->nombre, aux->desbloqueado,aux->marcas, 0);

            do
            {
                printw("\nCual marca logro: ");
                scanw("%19[^\n]s", respuesta);
                convertirMayuscula(respuesta);
                opcion = valorNumericoMarca(respuesta);
                if(opcion < 0 || opcion > 9) printw("\nNo existe tal marca\n");
            } while (opcion < 0 || opcion > 9);
            
            if(aux->marcas[opcion - 1] == 2) printw("\nLa marca ya se logro\n");
            else
            {
                
                do
                {
                    printw("\nLo realizo en ""NORMAL ""O ""DIFICIL"": ");
                    scanw("%19s", respuesta);
                    convertirMayuscula(respuesta);
                    if(strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0) printw("\nNo existe tal opcion\n");
                } while (strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0);
                
                if(strcmp("NORMAL", respuesta) == 0) aux->marcas[opcion] = 1;
                else aux->marcas[opcion] = 2;
            }
                
            mostrarPersonaje(aux->nombre, aux->desbloqueado,aux->marcas, 0);
        }
        else
        {
            attron(COLOR_PAIR(4));
            printw("\nEl personaje %s no se encuentra desbloqueado\n", aux->nombre);
            attroff(COLOR_PAIR(4));
        }
    }
    else
    {
        attron(COLOR_PAIR(3));
        attron(A_BOLD);
        printw("\nEl personaje ingresado no existe\n");
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
    }

    esperarTecla();
    endwin();
}