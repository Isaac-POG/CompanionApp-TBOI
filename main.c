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
		
		if(opcion1 == 0) break;
		
		do
		{
			mostrarMenuOpciones();
			scanf("%i", &opcion2);
			if(opcion2 == 0);
		}while(opcion2 != 0);
	
	}while(opcion1 < 0 || opcion1 > 4);

	return 0;
}