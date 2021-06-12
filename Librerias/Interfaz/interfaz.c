#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define red "\e[1;91m"
#define green "\e[1;92m" 
#define yellow "\e[1;93m"
#define blue "\e[1;94m"
#define cian "\e[1;96m"
#define purple "\e[1;95m"
#define reset "\e[0m"

void mostrarMenuOpciones()
{
	printf("\n1.)Menu de desbloqueo\n");
	printf("2.)Guardar Informacion\n");
	printf("3.)Buscar un Item Especifico\n");
	printf("4.)Buscar un Logro Especifico\n");
	printf("5.)Buscar un Enemigo Especifico\n");
	printf("6.)Mostrar a todos los Personajes\n");
	printf("7.)Mostrar todos los Items\n");
	printf("8.)Mostrar todos los Logros\n");
	printf("9.)Mostrar a todos los Enemigos\n\n");
	printf(red"0.)Salir del Programa\n"reset);
	printf("\nElija una opcion: ");
}

void mostrarMenuDesbloqueo()
{
	printf("\n1.)Desbloquear un Personaje\n");
	printf("2.)Avance en Marcas de logro\n");
	printf("3.)Encontre un Item\n");
	printf("4.)Desbloquear un Logro\n");
	printf("5.)Encontre un Enemigo\n\n");
	printf(red"0.)Salir del Menu\n"reset);
	printf("\nElija una opcion: ");
}	

void esperarEnter()
{
	char enter;
	printf("\nIngrese cualquier numero para continuar...  ");
	getchar();
	scanf("%c",&enter);
}

void mostrarMarcas(int i)
{
	printf(purple);
	switch (i)
	{
	case 0:
		printf("/Mom's Heart ");
		break;
	case 1:
		printf("/Isaac ");
		break;
	case 2:
		printf("/Boss Rush ");
		break;
	case 3:
		printf("/Satan ");
		break;
	case 4:
		printf("/??? ");
		break;
	case 5:
		printf("/The Lamb ");
		break;
	case 6:
		printf("/Mega Satan ");
		break;
	case 7:
		printf("/Ultra Greed ");
		break;
	case 8:
		printf("/Hush ");
		break;
	case 9:
		printf("/Deliriun ");
		break;
	}
}

void mostrarPersonaje(char * nombre, int * marcas)
{
	printf(yellow"\n%s ", nombre);
	for(int i = 0; i < 10; i++)
	{
		mostrarMarcas(i);
		if(marcas[i] == 0) printf(red"NO ");
		else if(marcas[i] == 1) printf(blue"NORMAL ");
		else printf(green"DIFICIL ");
		printf(reset);
	}
	printf("\n");
}

void convertirMayuscula(char * cadena)
{
	int largo = strlen(cadena);
	
	for(int i = 0; i < largo; i++)
	{
		cadena[i] = toupper(cadena[i]);
	}
}