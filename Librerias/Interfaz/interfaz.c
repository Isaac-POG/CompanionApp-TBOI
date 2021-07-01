#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void iniciarColores()
{
	initscr();

	start_color();

	//Letras blancas con fondo negro
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	
	//Letras verdes con fondo negro
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	
	//Letras rojas con fondo negro
	init_pair(3,COLOR_RED,COLOR_BLACK);

	//Letras cian con fondo negro
	init_pair(4,COLOR_CYAN,COLOR_BLACK);
}

void convertirMayuscula(char * cadena)
{
	int largo = strlen(cadena);
	
	for(int i = 0; i < largo; i++)
		cadena[i] = toupper(cadena[i]);
}

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

void mostrarMarcas(int i)
{
	initscr();

	attron(COLOR_PAIR(4));

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

	attroff(COLOR_PAIR(4));

	endwin();
}

void mostrarPersonaje(char * nombre, int * marcas)
{
	initscr();
	printw("\n%s ", nombre);
	for(int i = 0; i < 10; i++)
	{
		mostrarMarcas(i);
		if(marcas[i] == 0) printw("NO ");
		else if(marcas[i] == 1) printw("NORMAL ");
		else printw("DIFICIL ");
	}
	printw("\n");
	endwin();
}

void esperarTecla()
{
	initscr();
	
	attron(A_BOLD);
	attron(COLOR_PAIR(4));

	printw("\nApriete cualquier tecla para avanzar");
	getch();

	attroff(A_BOLD);
	attroff(COLOR_PAIR(4));

	clear();

	endwin();
}

void pantallaInicial()
{
	initscr();
	clear();

	FILE * archivo = fopen("Archivos/titulo.txt", "r");
	char lineaLeida[300];
	
	while(fgets(lineaLeida, 299, archivo))
	{
		printw("%s", lineaLeida);
	}

	fclose(archivo);

	esperarTecla();
	endwin();
}