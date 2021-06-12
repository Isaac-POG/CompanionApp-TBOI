#include <stdio.h>
#include <windows.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/personajes.h"

void menuDesbloqueo()
{
	int opcion;

	do
	{
		mostrarMenuDesbloqueo();
		scanf("%i", &opcion);
		printf(cls);
		if(opcion == 0) return;


		switch (opcion)
		{
		case 1:
			break;
		
		case 2:
			break;

		case 3:
			break;

		case 4:
			break;

		default:
			printf(red"\nLa opcion ingresada no existe\n"reset);
			break;
		}
		esperarEnter();
		printf(cls);
	}while(opcion != 0);
}

int main()
{
	int opcion;
	HashMap * mapaPersonajes = createMap(10);
	importarArchivoPersonajes(mapaPersonajes);
	
	do
	{
		mostrarMenuOpciones();
		scanf("%i", &opcion);
		printf(cls);
		if(opcion == 0) break;

		switch (opcion)
		{
			case 1:
				menuDesbloqueo();
				break;
			case 2:
				
				break;

			case 3:
				
				break;

			case 4:
				
				break;
			
			case 5:
				
				break;

			case 6:
				mostrarPersonajes(mapaPersonajes);
				break;

			case 7:
				
				break;

			case 8:
				
				break;

			case 9:
				
				break;

			default:
				printf(red"\nLa opcion ingresada no existe\n"reset);
				break;
		}
		esperarEnter();
		printf(cls);
	}while(opcion != 0);
	printf("\nFin del Programa\n\n");

	free(mapaPersonajes);

	return 0;
}