# cipher
### Cifraremos  los archivos aplicando la regla de los autómatas celulares a los bytes del archivo
 
 
Primero debemos daber cual es la longitud de bytes del archivo o el tamaño del buffer donde lo almacenaremos, para ello usamos la funcion 
   
    off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
    }
    
    

La cual llamamos en el main de la siguiente forma : file_len = fsize(archivo); 
una vez tenido el tamaño del archivo, debemos leerlo en forma de bytes, ya que modificando estos  los cifraremos,
declaramos tres variables para obtener nustros buffers, 
           
     char *buffer_old; 
	   char *buffer_new; 
	   char *buffer_aux; 
     
     buffer_old = malloc(file_len);   
     buffer_new = malloc(file_len);
     buffer_aux = malloc(file_len);
	 
 Estos buffers serán nuestros vectores de caracteres donde almacenaremos los bytes del archivo
 los declaramos como apuntadores para que utilicen memoria dinámica ya que los vectores tienen un limite de almacenamiento, lo cual       nos generaría problemas cuando quisiéramos encriptar un archivo con una alta longitud por ejemplo una canción o un video, al hacerlo asi podemos reservar espacio de memoria y evitar errores  


Ahora procedemos a almacenar los bytes del archivo en nuestro buffer llamado buffer_old;
para ellos hacemos un ciclo infinito que recorrerá  cada byte del archivo y a su vez lo ira almacenando en la posición I de nuestro buffer_old, con una validación que cierre el ciclo cuando de llegue al ultimo byte de este 

      	int i = 0;
	while(1) {
      		buffer_old[i] = fgetc(old_fd);
      		if (feof(old_fd)) {
         		break ;
      		}
		i++;
	}

