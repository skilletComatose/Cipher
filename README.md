# cipher
### Cifraremos  los archivos aplicando la regla de los autómatas celulares a los bytes del archivo

Para ello debemos leer los archivos en forma de bytes, ya que modificando estos  los cifraremos,
declaramos tres variables para obtener nustros buffers, 
           
     char *buffer_old; 
	   char *buffer_new; 
	   char *buffer_aux; 
     
     buffer_old = malloc(file_len);
   	 buffer_new = malloc(file_len);
     buffer_aux = malloc(file_len);
	 
 Estos buffers serán nuestros vectores de caracteres donde almacenaremos los bytes del archivo
 los declaramos como apuntadores para que utilicen memoria dinámica ya que los vectores tienen un limite de almacenamiento, lo cual       nos generaría problemas cuando quisiéramos encriptar un archivo con una alta longitud por ejemplo una canción o un video, al hacerlo asi podemos reservar espacio de memoria y evitar errores  

