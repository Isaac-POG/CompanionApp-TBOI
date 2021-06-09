#include <stdio.h>

#include "Interfaz/interfaz.h"

int main()
{
	int opcion1;
	int opcion2;
	do
	{
		mostrarMenuGuardado();
		scanf("%i", &opcion1);
		printf(cls);
		if(opcion1 == 0) break;
		
		do
		{
			mostrarMenuOpciones();
			scanf("%i", &opcion2);
			printf(cls);
			if(opcion2 == 0) break;
		}while(opcion2 != 0);

	}while(opcion1 > -1 && opcion1 < 4);

	printf("\nFin del Programa\n\n");

	return 0;
}