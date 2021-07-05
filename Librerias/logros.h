#ifndef LOGROS_H
#define LOGROS_H

//Calcula el porcentaje avanzado en los logros
float calculoTotalLogros(List *);

//Importa el archivo logros
void importarArchivoLogros(HashMap *, List *);

//Muestra todos los logros
void mostrarLogros(List *);

//Busca un logro ingresado por el usuario
void buscarLogroEspecifico(HashMap *, int);

//El usuario indica si desbloqueo el logro
void desbloquearLogro(HashMap *, int);

//Se guarda toda la informacion de los logros
void guardarInfoLogros(List *);

#endif