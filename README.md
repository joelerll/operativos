# Descripción del proyecto

Generar un analizador de código de python, recibirá el archivo y devolvera la salida

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
./bin/cliente.e test/dump/hello.c
```


## Dependencias

## Desarrollador

* Joel Rodriguez

# Primer parcial
* Pasar json entre cliente y servidor para enviar datos de respuesta
* El cliente debe recibir la respuesta (sockets streaming?) (Feature: junto con el id)
* Guardar datos en una tabla compartida userId, fecha, respuesta, rutaEjecutableFuente, rutaCodigoFuente, ordenLlegada

## Segundo Parcial

```txt
El proyecto se va a basar sobre el proyecto desarrollado en el primer parcial. Ahora el sistema va a contener un nuevo (s) módulo(s) que permitan realizar las siguientes acciones de forma concurrente con la ejecución de los programas de los usuarios.

M1) Monitorear continuamente la carga de los CPU(s)
M2) Al crear procesos asignarlos (affinidad) a los CPUs menos ocupados
M3) Detectar procesos que superen el tiempo de ejecucion Te
M4) Detectar la ocurrencia de major/minor page faults en el sistema
M5) proveer estadisticas acerca de los procesos involucrados
M6) Generar estadisticas de  procesos en ejecucion y de el sistema
  * uso de memoria
  * disco
  * cpu
  * tazas de procesos en ejecucion y terminados por minuto en un archivo log
M7) Permitir seleccionar pausar, terminar procesos
```