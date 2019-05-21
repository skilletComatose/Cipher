#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

void encrypt_file(const char *archivo, FILE *old_fd, unsigned int file_len) {
	
	char *buffer_old; //[file_len];
	char *buffer_new; //[file_len];
	char *buffer_aux; //[file_len];
	
	buffer_old = malloc(file_len);
	buffer_new = malloc(file_len);
    buffer_aux = malloc(file_len);
    
	int i = 0;
	while(1) {
      		buffer_old[i] = fgetc(old_fd);
      		if (feof(old_fd)) {
         		break ;
      		}
		i++;
	}

	
	
	FILE *new_fd;
	new_fd = fopen(archivo, "wb+");
	
	if (file_len > 0) {
		

		 for (int i=0; i<file_len; i++) buffer_aux[i] = i;
         
	  
      
      // Here is applicated the cellular automata (rule 39318)
      buffer_new[0] = buffer_old[0] ^ buffer_aux[1] ^ (buffer_aux[file_len-2] | buffer_aux[file_len-1]);
      buffer_new[1] = buffer_old[1] ^ buffer_aux[2] ^ (buffer_aux[file_len-1] | buffer_aux[0]);
      buffer_new[file_len-1] = buffer_old[file_len-1] ^ buffer_aux[0] ^ ( buffer_aux[file_len-3] | buffer_aux[file_len-2] );    // ESTE FUNCIONA

      for (int i = 2; i < file_len - 1 ; i++){
	 		buffer_new[i] = buffer_old[i] ^ buffer_aux[i+1] ^ ( buffer_aux[i-2] | buffer_aux[i-1]);
          }     
      
		
		fwrite(buffer_new, 1, file_len, new_fd);	

		printf("%s\n", buffer_new);
		fclose(new_fd);
	}

	
}

int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		printf("%s [file]", argv[0]);
		exit(0);	
	}
	
	FILE *old_file, *new_file;
	const char *archivo = argv[1];
	unsigned int file_len;
	
	old_file = fopen(archivo, "rb"); // read binary
	file_len = fsize(archivo); 

	encrypt_file(archivo, old_file, file_len);
   	fclose(old_file);
	return 0;
}
