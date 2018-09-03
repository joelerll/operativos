# Descripción del proyecto

Generar un analizador de código de c

## Instalación

1. Compilar los archivos

```sh
make
```

2. Correr el servidor

```sh
./bin/server
```

3. Enviar un archivo

```sh
./clients/client
```

4. Archivo log

El archivos de guardara en el archivo __log__


## Dependencias

## Desarrollador

* Joel Rodriguez

## Preguntas
* Las estadisticas son de todo el sistema o solo del programa?
* es un top el programa, debo guardar lo mismo de ahi?, cada cuanto tiempo deberia?z

# TODO
[x] enviar archivo del cliente al server
[x] probrar libreria de json
[x] obtener la carga de los cpus y mostralos en consola
[x] cambiado a procesos
[x] pruebas con shared memory o memory map (cual es mejor?)
[x] recoger variables en un array de procesos hijos e ir eliminando los terminados
  ademas agregar los procesos del exec (OJO)
[x] colocar un print con los procesos que se agregan (colocar un sleep de 1 segundo para cada procesos para ver como se eliminan)
[ ] reducir a un solo exec
  captar el stdout y stderr con los pipe
[x] cambiar popen por exec()
[x] obtener datos de todos los procesos del programa (
  [x] uso memoria, cat /proc/[pid]/statm
  [x] disco, /proc/[pid]/io
  [x] cpu, /proc/[pid]/stat obtener porcentajes de cpu por proceso?
  [x] Detectar la ocurrencia de major/minor page faults en el programa
  [x] guardar en un archivo log
[x] detectar procesos que superen un tiempo te
[x] guardar media, mediana, promedio (estaditicas basicas)
  * media
  * promedio de uso memoria
  * tasas de proceso en ejecucion x tiempo dado?
  [x] guardar en un archivo log
[x] Guardar datos en una tabla compartida userId, fecha, respuesta, rutaEjecutableFuente, rutaCodigoFuente, ordenLlegada
[ ] crear reporte

=========================================
[ ] mostrar los datos de tabla compartida
[ ] arreglar problema de fclose archivos porque si envio dos veces los archivos me bota ERROR
[ ] salen siempre al 0.0% los procesos
[ ] documento detallados con imagenes de como debe hacer las pruebas y que esta implementado
[ ] arreglar el cliente en python para que pueda enviar archivos
[ ] seleccionar, pausar, continuar y terminar procesos. Usar senales
[ ] generar reportes por medio de senales (Ej: mandar por senal que cada 5 segundos me genere reporte) o que genere el txt por medio senales


-- Extras
[ ] como hacer las pruebas de cada cosa?, como ingresaria esto?
[ ] enviar archivos y recibir respuesta por socket streming
[ ] servidor raw en python
[ ] diseno de pagina web para enviar codigo y recibir la respuestas (material design)
# FIX:

# Primer parcial
* Pasar json entre cliente y servidor para enviar datos de respuesta ?
* El cliente debe recibir la respuesta (sockets streaming?) (Feature: junto con el id) ?
## Segundo Parcial

```txt
El proyecto se va a basar sobre el proyecto desarrollado en el primer parcial. Ahora el sistema va a contener un nuevo (s) módulo(s) que permitan realizar las siguientes acciones de forma concurrente con la ejecución de los programas de los usuarios.

// BACKGROUND
M2) Al crear procesos asignarlos (affinidad) a los CPUs menos ocupados [x]
M3) Detectar procesos que superen el tiempo de ejecucion Te <------ Como hacer con hilos?
M4) Detectar la ocurrencia de major/minor page faults en el sistema ?

// MONITORING
M1) Monitorear continuamente la carga de los CPU(s) ?
M5) proveer estadisticas acerca de los procesos involucrados ?
M6) Generar estadisticas de  procesos en ejecucion y de el sistema ?
  * uso de memoria
  * disco
  * cpu
  * tazas de procesos en ejecucion y terminados por minuto en un archivo log ?

// DEL FRONT
M7) Permitir seleccionar pausar, terminar procesos
```

// extras
crear thead pool
crear servidor raw en python
posibilidad de facil switch entre threads y procesos

## Links utiles

https://www.tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.html

http://man7.org/linux/man-pages/man5/proc.5.html

http://5eonline.com/en/4-easy-ways-to-run-external-programs-in-cc/

https://www.gnu.org/software/libc/manual/html_node/Running-a-Command.html

https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf

http://nandal.in/2012/04/copy-file-using-c-threads/

https://web.archive.org/web/20150511104538/http://www.advancedlinuxprogramming.com/alp-folder/

https://overiq.com/c-programming/101/nested-structures-in-c/

https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/05/lec.html

https://gist.github.com/oleksiiBobko/43d33b3c25c03bcc9b2b

https://github.com/fho/code_snippets/blob/master/c/getusage.c

https://github.com/scaidermern/top-processes/blob/master/top_proc.c

https://www.linuxquestions.org/questions/programming-9/proc-stat-file-problem-for-cpu-usage-369302/

https://github.com/mbrossard/threadpool

https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf

https://www.pslinux.online/ps-examples.html

https://stackoverflow.com/questions/17006262/clearing-output-of-a-terminal-program-in-linux-c

http://www.cypress.com/file/54441/download

https://gist.github.com/ben-cohen/67d41a917ffbf7a5da99fd622feb346b

# OJO



El proyecto se va a basar sobre el proyecto desarrollado en el primer parcial. Ahora el sistema va a contener un nuevo (s) módulo(s) que permitan realizar las siguientes acciones de forma concurrente con la ejecución de los programas de los usuarios.



Monitorear continuamente la carga de los CPU(s) y al crear procesos asignarlos (affinidad) a los CPUs menos ocupados [x]

Detectar procesos que superen el tiempo de ejecucion Te [x]

Detectar la ocurrencia de major/minor page faults en el sistema [x]
proveer estadisticas acerca de los procesos involucrados

Generar estadisticas de uso de memoria,disco y cpu de los procesos en ejecucion, y las
tazas de procesos en ejecucion y terminados por minuto.
Estos datos deben de registrarse en un archivo de log.

Permitir seleccionar procesos a pausar o terminar afin de que el adminsitrador pueda mejorar el tiempo de respuesta del sistema.

<!--
// limpiar los procesos terminados
// pthread_mutex_lock(&mapping->mutex);
// 	pid_t pmaster = getpid();
// 	u_delete_number(mapping->array, mapping->cantidad, pmaster);
// 	mapping->cantidad--;
// pthread_mutex_unlock(&mapping->mutex);
// limpiar los procesos terminados -->

<!-- // char * line = NULL;
// size_t len = 0;
// char **datos = malloc(sizeof(char *) * 8);
// for (size_t i = 0; i < 8; i++) {
// 	ssize_t read = getline(&line, &len, fp);
// 	if (read != -1) {
// 		char **s = u_split(line, ' ');
// 		char *palabraTmpNew = malloc(sizeof(char *));
// 		strcpy(palabraTmpNew, s[1]);
// 		datos[i] = palabraTmpNew;
// 	}
// }
// io = proc_io_crear(datos[0], datos[1], datos[2], datos[3], datos[4], datos[5], datos[6]); -->
