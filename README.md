# The Binding of Isaac: Companion App

![image](https://sm.ign.com/ign_es/screenshot/default/the-binding-of-isaac-rebirth-afterbirth-3591010_pyut.jpg)

Aplicación del **Grupo James Bond**, para poder almacenar los avances realizados en el juego *The Binding of Isaac: Afterbirth+* y que además permite al usuario poder revisar la informacion respectiva de los personajes, ítems, logros y enemigos.

## Integrantes

---

- José Osega, como aspectos positivos, trabajó desde el principio, optimista y vio varias posibilidades de implementar un programa bonito.
- Bryan López, como aspectos positivos, entendio de forma rápida la incorporación de la libreria **ncurses** al programa, dio ideas de como utilizar la misma en otras funciones, buscando que quedará mas bonito el programa. Como punto a mejorar, debe de confiar mas en sus habilidades al programar y en la de su compañero.

### Requisitos para poder compilar el programa

---

- Es necesario usar la libreria ncurses, que se puede instalar en Debian y sus derivados con el siguiente comando *"sudo apt-get install libncurses5-dev libncursesw5-dev"*.
- Se puede instalar en Windows a través de WSL o Windows Subsystem for Linux, más informacion en <https://docs.microsoft.com/en-us/windows/wsl/install-win10>.

### Linea de Compilador

---

- **gcc -o programa Librerias/TDA_Mapa/hashmap.c Librerias/TDA_Lista/list.c Librerias/Estructuras/structs.h Librerias/Interfaz/interfaz.c Librerias/personajes.c Librerias/items.c Librerias/logros.c Librerias/enemigos.c main.c -lm -lncurses**
