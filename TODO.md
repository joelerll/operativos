https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c#5656561

[ ] hacer un servidor multiprocesos. Cliente servidor
	* el cliente envia el archivo o cualquier cosa
	* el server recibe el archivo y lo manda a otro proceso
	* el servidor recibe el archivo y lo ejecuta

[ ] generar una tabla compartida en memoria ram y guardar en archivo


[ ] generar comandos por medio de senales para pruebas de stress 


[ ] script con .h en c para ejecutar un archivo y que devuelva el resultado [Jueves, Viernes]



[ ] Registrar un usuario
[ ] Enviar el archivo de texto al usuario
[ ] Ejecutar el programa enviado
[ ] Recibir el programa ejecutado
[ ] Sumitir el programa para la evaluacion

cuando reciba el archivo en un hilo guardarlo y en otro ejecutarlo,
luego esperar a que se guarde y que se ejecute


1) El archivo debe grabarse en el disco duro
2) El programa debe ejecutarse de forma independiente y la salida debe guardarse en una tabla compartida entre todos los procesos
3) Si el programa ejecutable no existe, el programa fuente debe compilarse y ejecutarse en un proceso independiente?
4) La integridad de los datos de la tabla de respuestas debe estar garantizada. Debe almacenarse en ram y una por cliente, 
identificador, usuario, fecha de envio, respuesta, ruta al programa ejecutable y fuente, orden de llegada
5) Cliente debe permanecer cargado en memoria y recibir comandos por senales para poder realizar pruebas de stress
6) Otra cosa relevante

Discusiones?
Modo de comunicacion entre procesos clientes y server: implementacion de tal manera que pueda cambiarse facilmente de los dos metodos
	* Sockets
		* un cliente
		* multiples clientes
		* thread pool
	* Senales


Ejecutar el codigo a el programa fuente que se pide
	* C, facil implementecion y cambio de manejador
	* Python

Modo de guardar los datos
	* tabla hash, que estructura debe tener?

Al iniciarce el programa debe cargar los datos

Modo de guardar los datos para que puedan ser recuperados los datos

Debe tener varios componentes 

#### Cosas extras

* Thread pool, para mayor perfomance
* Script makefile de instalacion de dependencias
* 