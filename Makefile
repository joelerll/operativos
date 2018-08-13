all: main

main:
	gcc -Wall src/main.c -o bin/main -pthread

tabla.o: src/tabla.c
	gcc -c -Wall -Iinclude/ src/tabla.c -o obj/tabla.o

server: src/socket.server.c obj/tabla.o
	gcc -Wall -Iinclude/ -pthread src/socket.server.c obj/tabla.o -o bin/server
	./bin/server

client: src/socket.client.c
	gcc  -Wall -Wextra src/socket.client.c -o bin/client -pthread
	./bin/client test/dump/hello.c

prueba: src/prueba.c tabla.o
	gcc  -pthread -Iinclude/ obj/tabla.o src/prueba.c  -o prueba
