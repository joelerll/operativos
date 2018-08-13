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

## Segundo Parcial

```txt
El proyecto se va a basar sobre el proyecto desarrollado en el primer parcial. Ahora el sistema va a contener un nuevo (s) módulo(s) que permitan realizar las siguientes acciones de forma concurrente con la ejecución de los programas de los usuarios.



* Monitorear continuamente la carga de los CPU(s) y al crear procesos asignarlos (affinidad) a los CPUs menos ocupados

* Detectar procesos que superen el tiempo de ejecucion Te

* Detectar la ocurrencia de major/minor page faults en el sistema y proveer estadisticas acerca de los procesos involucrados

* Generar estadisticas de uso de memoria,disco y cpu de los procesos en ejecucion y de el sistema, y las tazas de procesos en ejecucion y terminados por minuto. Estos datos deben de registrarse en un archivo de log.

* Permitir seleccionar procesos a pausar o terminar afin de que el adminsitrador pueda mejorar el tiempo de respuesta del sistema.
```
