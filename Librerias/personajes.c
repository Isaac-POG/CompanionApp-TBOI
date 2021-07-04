#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "TDA_Mapa/hashmap.h"
#include "TDA_Lista/list.h"
#include "Interfaz/interfaz.h"
#include "Estructuras/structs.h"

//Funcion para calcular el avance de las marcas de cada personaje
int calculoMarcas(tipoPersonaje * personajeActual)
{
    int calculoMarcas = 0;
    
    //Se recorre el arreglo marcas
    for(int i = 0; i < 10; i++)
    {
        if(personajeActual->marcas[i] == 1) calculoMarcas += 1; //Dificultad normal
        else if (personajeActual->marcas[i] == 2) calculoMarcas += 2; //Dificultad dificil
    }

    return calculoMarcas;
}

float calculoTotalPersonajes(List * listaPersonajes)
{
    tipoPersonaje * aux = firstList(listaPersonajes);
    float contadorTotal = 0;

    while(aux != NULL)
    {
        if(aux->desbloqueado == 1)
        {
            contadorTotal++; //1 punto por tener desbloqueado al personaje
            contadorTotal += calculoMarcas(aux);
        }
        aux = nextList(listaPersonajes);
    }

    return contadorTotal;
}

//Se separa la informacion del personaje y se almacena en un tipoPersonaje
tipoPersonaje * copiarInformacionPersonaje(char * lineaLeida)
{
    tipoPersonaje * nuevoPersonaje = malloc (sizeof(tipoPersonaje));
	char * fragmento;

	//Nombre Personaje
	fragmento = strtok(lineaLeida, ",");
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
    //Se abre el archivo que almacena la informacion
    FILE * archivo = fopen("Archivos/personajes.txt", "r");
    if(archivo == NULL) return;

    char lineaLeida[100];

    //Se elimina la primera linea
    fgets(lineaLeida, 99, archivo);

    //Se copia la informacion en el mapa y en la lista
    while(fgets(lineaLeida, 99, archivo))
    {
        tipoPersonaje * nuevoPersonaje = copiarInformacionPersonaje(lineaLeida);
        insertMap(mapaPersonajes, nuevoPersonaje->nombre, nuevoPersonaje);
        pushBack(listaPersonajes, nuevoPersonaje);
    } 

    //Se cierra el archivo
    fclose(archivo);
}

//Se guardan los cambios realizados durante la ejecucion del programa, y estos cambios se guardan en personajes.txt
void guardarInfoPersonajes(List * listaPersonajes)
{
    //Se abre el archivo que contiene a los personajes
    FILE * archivo = fopen("Archivos/personajes.txt", "w");
    if(archivo == NULL) return;
    
    //Se copia la primera linea
    fprintf(archivo,"Personaje,Desbloqueado,Marcas\n");
    tipoPersonaje * aux = firstList(listaPersonajes);

    //Se recorre la lista para copiar la información actualizada
    while(aux != NULL)
    {
        fprintf(archivo, "%s,%i", aux->nombre, aux->desbloqueado);
        for(int j = 0; j < 10; j++)
        {
            fprintf(archivo, ",%i", aux->marcas[j]);
        }
        fprintf(archivo,"\n");
        aux = nextList(listaPersonajes);
    }

    //Se cierra el archivo
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

    //Se crea la opcion para salir del menu
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
                esperarTecla(0);
            }
            else return;
            break;
        }

    }
    endwin();
}

//Función que actualiza la informacion de desbloqueo de un personaje
void desbloquearPersonajes(List * listaPersonajes, HashMap * mapaPersonajes)
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

    //Si estan todos los personajes desbloqueados
    if(j == 0)
    {
        printw("Ha desbloqueado todos los personajes\n");
        esperarTecla(0);
        endwin();
        return;
    }

    //Se crea la opcion salir menu
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

        switch (eleccion) //Dependiendo de la tecla ingresada
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
            if(iluminar != j - 1) //Si el usuario no se encuentra en la ultima opcion
            {
                //Se desbloquea al personaje
                tipoPersonaje * nuevoPersonaje = searchMap(mapaPersonajes, personajes[iluminar]);
                nuevoPersonaje->desbloqueado = 1;
                clear();

                //Mensaje de desbloqueo con colores
                attron(COLOR_PAIR(5));
                printw("%s",nuevoPersonaje->nombre);
                attroff(COLOR_PAIR(5));
                
                printw(" ahora se encuentra ");
                
                attron(COLOR_PAIR(2));
                attron(A_BOLD);
                printw("DESBLOQUEADO");
                attroff(COLOR_PAIR(2));
                attroff(A_BOLD);

                esperarTecla(0);
            }
            endwin();
            return;
        }

    }
}

//Funcion para actualizar la informacion respecto al avance en las marcas de logro de un personaje
void avanceMarcasLogros(List * listaPersonajes, HashMap * mapaPersonajes)
{
    char personajes[16][25];
    char respuesta[20];
    int opcion;
    int j = 0, eleccion = -1, iluminar = 0;

    tipoPersonaje * auxRecorrido = firstList(listaPersonajes);
    
    //Se almacena en un arreglo de cadenas, todos los personajes desbloqueados y que no esten completados
    while(auxRecorrido != NULL)
    {
        if(auxRecorrido->desbloqueado == 1 && calculoMarcas(auxRecorrido) != 20) 
        {
            strcpy(personajes[j], auxRecorrido->nombre);
            j++;
        }
        auxRecorrido = nextList(listaPersonajes);
    }

    //Si no se puede avanzar más con los personajes actuales
    if(j == 0)
    {
        printw("Ha logrado avanzar en marcas de logros con todos los personajes desbloqueados\n");
        esperarTecla(0);
        endwin();
        return;
    }

    //Se crea una opcion para salir del menu
    strcpy(personajes[j], "Salir del Menu");
    j++;

    //Se crea la ventana que contendra al menu
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

        switch (eleccion) //Dependiendo de la tecla apretada
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
            if(iluminar != j - 1) //Si no es la opcion salir menu
            {
                //Se busca el personaje en el mapa, y se muestra por pantalla
                tipoPersonaje * nuevoPersonaje = searchMap(mapaPersonajes, personajes[iluminar]);
                clear();
                mostrarPersonaje(nuevoPersonaje->nombre, nuevoPersonaje->desbloqueado,nuevoPersonaje->marcas, 0);
                
                //El usuario ingresa el logro que quiere actualizar
                do
                {
                    printw("\nNombre de la marca que logro: ");
                    scanw("%19[^\n]s", respuesta);
                    convertirMayuscula(respuesta);
                    opcion = valorNumericoMarca(respuesta);
                    if(opcion < 0 || opcion > 9) printw("\nNo existe tal marca\n");
                } while (opcion < 0 || opcion > 9);
            
                //Si se completo en su maxima dificultad, la funcion termina
                if(nuevoPersonaje->marcas[opcion] == 2)
                {
                    printw("\nLa marca ya se completo en su maxima dificultad\n");
                    esperarTecla(0);
                    endwin();
                    return;
                }
                else //Si no, se pregunta en que dificultad se hizo, para actualizarlo y mostrarlo por pantalla
                {
                    do
                    {
                        printw("\nLo realizo en ");
                        
                        attron(COLOR_PAIR(5));
                        printw("NORMAL ");
                        attroff(COLOR_PAIR(5));
                        
                        printw("O ");

                        attron(COLOR_PAIR(5));
                        printw("DIFICIL"": ");
                        attroff(COLOR_PAIR(5));
                        
                        scanw("%19s", respuesta);
                        convertirMayuscula(respuesta);
                        if(strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0) printw("\nNo existe tal opcion\n");
                    } while (strcmp(respuesta, "NORMAL") != 0 && strcmp(respuesta,"DIFICIL") != 0);
                    
                    if(strcmp("NORMAL", respuesta) == 0) nuevoPersonaje->marcas[opcion] = 1;
                    else nuevoPersonaje->marcas[opcion] = 2;
                }
                
                clear();
                mostrarPersonaje(nuevoPersonaje->nombre, nuevoPersonaje->desbloqueado,nuevoPersonaje->marcas, 0);
                esperarTecla(0);
            }
            endwin();
            return;
        }
    }
}