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
./bin/cliente test/dump/hello.c
```


## Dependencias

## Desarrollador

* Joel Rodriguez

# TODO
[x] enviar archivo del cliente al server
[x] probrar libreria de json
[ ] obtener la carga de los cpus
[ ] obtener datos de todos los hilos (uso memoria, disco, cpu, tasas de proceso en ejecucion x tiempo), guardarlo en un .log
[ ] obtener datos de todos los procesos de la pc
[ ] como mostrar esos datos?

[ ] detectar hilos que superen un tiempo te
[ ] seleccionar, pausar, continuar y terminar procesos
[ ] como hacer las pruebas de cada cosa?

[ ] enviar archivos y recibir respuesta por socket streming
[ ] implementar thread pool
[ ] servidor raw en python
[ ] diseno de pagina web para enviar codigo y recibir la respuestas (material design)

# Primer parcial
* Pasar json entre cliente y servidor para enviar datos de respuesta ?
* El cliente debe recibir la respuesta (sockets streaming?) (Feature: junto con el id) ?
* Guardar datos en una tabla compartida userId, fecha, respuesta, rutaEjecutableFuente, rutaCodigoFuente, ordenLlegada

## Segundo Parcial

```txt
El proyecto se va a basar sobre el proyecto desarrollado en el primer parcial. Ahora el sistema va a contener un nuevo (s) módulo(s) que permitan realizar las siguientes acciones de forma concurrente con la ejecución de los programas de los usuarios.

// BACKGROUND
M2) Al crear procesos asignarlos (affinidad) a los CPUs menos ocupados [x]
M3) Detectar procesos que superen el tiempo de ejecucion Te
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

## Links utiles

http://5eonline.com/en/4-easy-ways-to-run-external-programs-in-cc/

https://www.gnu.org/software/libc/manual/html_node/Running-a-Command.html

https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf

http://nandal.in/2012/04/copy-file-using-c-threads/
