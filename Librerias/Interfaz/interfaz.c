#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Funcion para crear el espacio usado por el menu
WINDOW * crearVentana(int cantOpciones)
{
	//Limpiar pantalla
	clear();

	//Dependiendo del tamaño de la terminal, ajustarlo para nuestro uso y centrarlo
    WINDOW * ventana = newwin(cantOpciones, COLS/2, (LINES - cantOpciones)/2, (COLS)/4);

    //Activar las teclas de flechas
    keypad(ventana, TRUE);
	return ventana;
}

//Funcion para iniciar los colores que usamos en el programa
void iniciarColores()
{
	initscr();

	start_color();
	
	//Letras verdes con fondo negro
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	
	//Letras rojas con fondo negro
	init_pair(3,COLOR_RED,COLOR_BLACK);

	//Letras cian con fondo negro
	init_pair(4,COLOR_CYAN,COLOR_BLACK);

	//Letras magenta con fondo negro
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	endwin();
}

//Funcion para convertir cualquier cadena en una cadena compuesta de puras mayusculas
void convertirMayuscula(char * cadena)
{
	int largo = strlen(cadena);
	
	for(int i = 0; i < largo; i++)
		cadena[i] = toupper(cadena[i]);
}

//Funcion para indicar la posicion de la marca de logro
int valorNumericoMarca(char * nombreMarca)
{
	convertirMayuscula(nombreMarca);
	if(strcmp(nombreMarca, "MOM'S HEART") == 0)
	{
		return 0;
	}
	else if(strcmp(nombreMarca, "ISAAC") == 0)
	{
		return 1;
	}
	else if(strcmp(nombreMarca, "BOSS RUSH") == 0)
	{
		return 2;
	}
	else if(strcmp(nombreMarca, "SATAN") == 0)
	{
		return 3;
	}
	else if(strcmp(nombreMarca, "???") == 0)
	{
		return 4;
	}
	else if(strcmp(nombreMarca, "THE LAMB") == 0)
	{
		return 5;
	}
	else if(strcmp(nombreMarca, "MEGA SATAN") == 0)
	{
		return 6;
	}
	else if(strcmp(nombreMarca, "ULTRA GREED") == 0)
	{
		return 7;
	}
	else if(strcmp(nombreMarca, "HUSH") == 0)
	{
		return 8;
	}
	else if(strcmp(nombreMarca, "DELIRIUM") == 0)
	{
		return 9;
	}
	else return 10;
}

//Funcion para mostrar los nombres de las marcas de logro
void mostrarMarcas(int i)
{
	initscr();

	attron(COLOR_PAIR(5));
	attron(A_ITALIC);

	switch (i)
	{
	case 0:
		printw("/Mom's Heart ");
		break;
	case 1:
		printw("/Isaac ");
		break;
	case 2:
		printw("/Boss Rush ");
		break;
	case 3:
		printw("/Satan ");
		break;
	case 4:
		printw("/??? ");
		break;
	case 5:
		printw("/The Lamb ");
		break;
	case 6:
		printw("/Mega Satan ");
		break;
	case 7:
		printw("/Ultra Greed ");
		break;
	case 8:
		printw("/Hush ");
		break;
	case 9:
		printw("/Delirium ");
		break;
	}

	attroff(COLOR_PAIR(5));
	attroff(A_ITALIC);

	endwin();
}

//Funcion para mostrar de dos formas la informacion de los personajes
void mostrarPersonaje(char * nombre, int desbloqueado, int * marcas, int tipoMostrar)
{
	initscr();

	printw("%s ", nombre);

	//Segun la funcion que necesita mostrar la informacion se muestra de una u otra forma	
	if(tipoMostrar == 0)
	{
		for(int i = 0; i < 10; i++)
		{
			mostrarMarcas(i);
			if(marcas[i] == 0)
			{
				attron(COLOR_PAIR(3));
				printw("NO ");
				attroff(COLOR_PAIR(3));
			} 
			else if(marcas[i] == 1) 
			{
				attron(COLOR_PAIR(4));
				printw("NORMAL ");
				attroff(COLOR_PAIR(4));
			}
			else
			{
				attron(COLOR_PAIR(2));
			 	printw("DIFICIL ");
			 	attroff(COLOR_PAIR(2));
			}
		}
	}
	else
	{
		if(desbloqueado == 0)
		{
			attron(COLOR_PAIR(3));
			printw("BLOQUEADO ");
			attroff(COLOR_PAIR(3));
		} 
		else 
		{
			attron(COLOR_PAIR(2));
			printw("DESBLOQUEADO ");
			attroff(COLOR_PAIR(2));
		}
		
		printw("\n\n");
		for(int i = 0; i < 10; i++)
		{
			mostrarMarcas(i);
			if(marcas[i] == 0)
			{
				attron(COLOR_PAIR(3));
				printw("NO ");
				attroff(COLOR_PAIR(3));
			} 
			else if(marcas[i] == 1) 
			{
				attron(COLOR_PAIR(4));
				printw("NORMAL ");
				attroff(COLOR_PAIR(4));
			}
			else
			{
				attron(COLOR_PAIR(2));
			 	printw("DIFICIL ");
			 	attroff(COLOR_PAIR(2));
			}
			printw("\n");
		}
	}
	printw("\n");
	endwin();
}

void centrarEnY(int cantLineas)
{
	int i;
	//Bajar la cantidad de lineas necesarias para llegar al centro en Y
	for(i = 0; i < (LINES - cantLineas)/2 - 1;i++)
		printw("\n");
}

void centrarEnX(int largoPalabra)
{
	int i;
	for(i = 0; i < (COLS - largoPalabra)/2;i++)
		printw(" ");
}

//Funcion para mostrar la pantalla de inicio del programa
void pantallaInicial()
{
	initscr();
	int largoPalabra = strlen("The Binding of Isaac");
	
	centrarEnY(0);
	centrarEnX(largoPalabra);

	attron(A_BOLD);
	
	printw("The Binding of Isaac\n");

	largoPalabra = strlen("Companion App");
	centrarEnX(largoPalabra);

	attron(COLOR_PAIR(3));
	printw("Companion App\n\n");
	attroff(COLOR_PAIR(3));

	largoPalabra = strlen("Apriete cualquier tecla para iniciar el programa");
	centrarEnX(largoPalabra);

	attron(COLOR_PAIR(4));
	printw("Apriete cualquier tecla para iniciar el programa");
	attroff(COLOR_PAIR(4));

	attroff(A_BOLD);
	getch();
	clear();
	endwin();
}

//Funcion para mostrar la pantalla final del programa
void pantallaFinal()
{
	clear();
	int largoPalabra = strlen("Fin del Programa");
	int i;

	centrarEnY(0);
	centrarEnX(largoPalabra);

	attron(COLOR_PAIR(3));
	printw("Fin del Programa\n");
	attroff(COLOR_PAIR(3));
	
	largoPalabra = strlen("Gracias por usar el programa!");
	
	centrarEnX(largoPalabra);
	
	attron(COLOR_PAIR(4));
	printw("Gracias por usar el programa!");
	attroff(COLOR_PAIR(4));
	
	getch();
}

//Funcion para esperar que el usuario avance
void esperarTecla(int opcion)
{
	initscr();
	
	//Se pone la letra en negrita y en color cian
	attron(A_BOLD);
	attron(COLOR_PAIR(4));

	//Centrar el texto cuando se necesite
	if(opcion == 1) centrarEnX(strlen("Apriete cualquier tecla para avanzar"));
	else printw("\n");

	//Espera que se apriete cualquier tecla
	printw("Apriete cualquier tecla para avanzar");
	getch();

	attroff(A_BOLD);
	attroff(COLOR_PAIR(4));

	//Limpia la pantalla
	clear();

	endwin();
}