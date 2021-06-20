#include <stdio.h>
#include <stdlib.h>

#include "Librerias/Interfaz/interfaz.h"
#include "Librerias/TDA_Mapa/hashmap.h"
#include "Librerias/personajes.h"
#include "Librerias/logros.h"
#include "Librerias/items.h"

void menuDesbloqueo(HashMap * mapaPersonajes,HashMap *mapaLogros, HashMap * mapaItems)
{
	int opcion;
	char nombreBuscado[40];
	int idBuscar;
	do
	{
		mostrarMenuDesbloqueo();
		scanf("%i", &opcion);
		printf(cls);
		if(opcion == 0) return;

		switch (opcion)
		{
		case 1:
			mostrarNombres();
			printf("\nIngrese el nombre del personaje: ");
			getchar();
			scanf("%39[^\n]s", nombreBuscado);
			convertirMayuscula(nombreBuscado);			
			desbloquearPersonajes(mapaPersonajes, nombreBuscado);
			esperarEnter();
			break;
		
		case 2:
			mostrarNombres();
			printf("\nIngrese el nombre del personaje: ");
			getchar();
			scanf("%39[^\n]s", nombreBuscado);
			convertirMayuscula(nombreBuscado);
			avanceMarcasLogros(mapaPersonajes, nombreBuscado);			
			esperarEnter();
			break;

		case 3:
			printf("\nIngrese el nombre del item: ");
			getchar();
			scanf("%39[^\n]s", nombreBuscado);
			convertirMayuscula(nombreBuscado);
			encontrarItem(mapaItems, nombreBuscado);			
			esperarEnter();
			break;

		case 4:
			printf("Ingrese el ID del logro a desbloquear: ");
			scanf("%d",&idBuscar);
			desbloquearLogro(mapaLogros, idBuscar);
			esperarEnter();
			break;

		case 5:
			break;

		default:
			printf(red"\nLa opcion ingresada no existe\n"reset);
			break;
		}
		printf(cls);
	}while(opcion != 0);
}

int main()
{
	pantallaInicial();

	int opcion;
	int idBuscar;
	char nombreBuscado[40];
	HashMap * mapaPersonajes = createMap(10);
	HashMap * mapaLogros = createMap(576);
	HashMap * mapaItems = createMap(600);
	
	importarArchivoPersonajes(mapaPersonajes);
	importarArchivoLogros(mapaLogros);
	importarArchivoItems(mapaItems);	
	do
	{
		mostrarMenuOpciones();
		scanf("%i", &opcion);
		printf(cls);
		if(opcion == 0) break;

		switch (opcion)
		{
			case 1:
				menuDesbloqueo(mapaPersonajes,mapaLogros, mapaItems);
				break;

			case 2:
				guardarInfoPersonajes(mapaPersonajes);
				break;

			case 3:
				printf("\nIngrese el nombre del item: ");
				getchar();
				scanf("%39[^\n]s", nombreBuscado);
				convertirMayuscula(nombreBuscado);			
				buscarItemEspecifico(mapaItems, nombreBuscado);
				esperarEnter();
				break;

			case 4:
				printf("Ingrese el ID del logro a buscar: ");
				scanf("%d",&idBuscar);
				buscarLogroEspecifico(mapaLogros, idBuscar);
				esperarEnter();
				break;
			
			case 5:
				
				break;

			case 6:
				mostrarPersonajes(mapaPersonajes);
				esperarEnter();
				break;

			case 7:
				mostrarTodosItems(mapaItems);
				esperarEnter();
				break;

			case 8:
				mostrarLogros(mapaLogros);
				esperarEnter();
				break;

			case 9:
				
				break;

			default:
				printf(red"\nLa opcion ingresada no existe\n"reset);
				break;
		}
		printf(cls);
	}while(opcion != 0);
	printf("\nFin del Programa\n\n");

	free(mapaPersonajes);
	free(mapaLogros);

	return 0;
}