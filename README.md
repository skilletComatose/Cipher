# CIPHER

# Autómata celular de una dimensión

Los autómatas celulares en una dimensión fueron explorados ampliamente en el libro ‘a new kind of science’ de Stephen Wolfram.
el estado de dichos autómatas consiste en un vector de valores binarios. En cada cambio de estado del autómata, cada valor cambia en función de su estado actual y de sus " vecinos o vecindades "

### Cifraremos  los archivos aplicando la regla de los autómatas celulares a los bytes del archivo
 
 
Primero debemos saber cual es la longitud de bytes del archivo o el tamaño del buffer donde lo almacenaremos, para ello usamos la funcion 
   
    off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
    }
    
    

La cual llamamos en el main de la siguiente forma : file_len = fsize(archivo); 
una vez tenido el tamaño del archivo, debemos leerlo en forma de bytes, ya que modificando estos  los cifraremos,
declaramos tres variables para obtener nustros buffers, 
           
        char* file_bytes;
	char* file_encrypted_bytes;
	char* bytes_random;
	
	file_bytes = malloc(file_len + 1);
	file_encrypted_bytes = malloc(file_len + 1);
	bytes_random = malloc(file_len + 1);
	 
 Estos buffers serán nuestros vectores de caracteres donde almacenaremos los bytes del archivo
 los declaramos como apuntadores para que utilicen memoria dinámica ya que los vectores tienen un limite de almacenamiento, lo cual       nos generaría problemas cuando quisiéramos encriptar un archivo con una alta longitud por ejemplo una canción o un video, al hacerlo asi podemos reservar espacio de memoria y evitar errores. file_bytes es donde se almacenan los bytes del archivo, file_encrypted_bytes es el buffer donde se almacenaran los bytes encriptados,  bytes_random es el buffer auxiliar que con el que haremos las modificaciones a los bytes del archivo


Ahora procedemos a almacenar los bytes del archivo en nuestro buffer llamado file_bytes
para ellos hacemos un ciclo infinito que recorrerá  cada byte del archivo y a su vez lo ira almacenando en la posición I de nuestro file_bytes , con una validación que cierre el ciclo cuando de llegue al ultimo byte de este 

	int i = 0;
	while (1) {
		file_bytes[i] = fgetc(file_fd);
		if (feof(file_fd)) {
			break;
		}
		i++;
	}


utilizamos un puntero que permita escribir en el archivo de forma binaria 
        
	FILE *new_fd;
	new_fd = fopen(archivo, "wb+");

	
## A) Movimiento entre vecindades 

Procedemos a aplicar la reglas para un autómata celular de una dimensión mas específicamente a interactuar con sus vecindades , para cada byte del buffer nos desplazaremos  dos vecindades hacia la izquierda y una vecindad hacia la derecha,pero si hacemos esto de forma directa se desbordaría el buffer, por ejemplo
        
	+-----------+ 
	|   BUFFER  |
	+-----------+ 
	| 1 | 2 | 3 |
	+---+---+---+
	| 4 | 5 | 6 |     ---> esquema grafico del buffer 
	+---+---+---+
	| 7 | 8 | 9 |
	+---+---+---+
	
supongamos que ese es nuestro buffer, el cual tiene forma de matix, si  nos colocamos en la posición [0] esta seria donde se encuentra el numero 1 , si queremos desplazarnos 2 Posiciones  hacia la izquierda se desbordaría, ya que a la izquierda no se encuentra  ningún valor, entonces usamos un truco para evitar esto, si a la longitud del buffer le restamos 1, seria 9 -1 = 8, sabiendo que los vectores empiezan a contar desde la posición cero, la posición 8 le correspondería al valor 9 osea que de la posición cero de desplazo a hacia la izquierda una vecindad, ya sabiendo este truco solo vasta con identificar que posiciones se podrían desbordar y aplicarles dicho truco, la posiciones serian la posición [0] , [1] , y la ultima posocion, 

## B) Principio para cifrar usando el movimiento de las vecindades

Ahora que sabemos como hacer los movimientos entre las vecindades usaremos estos movimientos para cifrar el archivo, haciendo que cada posición haga un XOR con la vecindad que tiene a su derecha, y a ese resultado se le hará otro XOR con el valor resultante de la operación OR entre las dos vecindades hacia la izquierda  
        
          	   
	   +>>>>>>>>>>>>>>>>>>> ejemplo ilustrativo <<<<<<<<<<<<<<<<<<<<<+
	   | 	                                                         |
	   |   1) ^   ----> simboliza la operación XOR                   |
	   |   2) |   ----> simboliza la operación OR                    |
	   |                                                             |
	   |   buffer[i] ^ buffer[i +1]  ^ ( buffer[i-2] | buffer[i-1] ) |  
	   |                                                             |
	   +>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>+
	 
Las operaciones mostradas anterior mente (XOR y OR) no se harán con el contendido del mismo buffer_old(el que tiene guardado los bytes del archivo)  sino que   utilizaremos un buffer auxiliar con la misma londitud de nuestro buffer_old  , y lo llenaremos con valores arbitrarios,  y estos valores serán los que utilizaremos para modificar mediante la operaciones lógicas antes mencionadas los bytes almacenados en buffer_old

## C) Implementación del cifrado por medio de vecindades 
Teniendo el cuenta lo mencionado en el inciso A y B entonces nuestro código nos queda : 
         
		
		file_encrypted_bytes[0] = file_bytes[0] ^ bytes_random[1] ^ (bytes_random[file_len - 2] | bytes_random[file_len - 1]);
		file_encrypted_bytes[1] = file_bytes[1] ^ bytes_random[2] ^ (bytes_random[file_len - 1] | bytes_random[0]);
		file_encrypted_bytes[file_len - 1] = file_bytes[file_len - 1] ^ bytes_random[0] ^ (bytes_random[file_len - 3] | bytes_random[file_len - 2]);

		for (int i = 2; i < file_len - 1; i++)
			file_encrypted_bytes[i] = file_bytes[i] ^ bytes_random[i + 1] ^ (bytes_random[i - 2] | bytes_random[i - 1]);
