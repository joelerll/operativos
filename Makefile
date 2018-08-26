.PHONY: clean

all: main

# main:
# 	cd parson
# 	make
# 	cd ..
	# gcc -Wall src/main.c -o bin/main -pthread

parson.o: parson/parson.c
	gcc -c -Wall -Iparson/ parson/parson.c -o obj/parson.o

tabla.o: src/tabla.c
	gcc -c -Wall -Iinclude/ src/tabla.c -o obj/tabla.o

utils.o: src/utils.c
	@gcc -c -Wall -Iinclude/ src/utils.c -o obj/utils.o

proc.o: src/proc.c
	@gcc -c -Wall -Iinclude/ src/proc.c -o obj/proc.o

server: src/socket.server.c obj/tabla.o
	gcc -Wall -Iinclude/ -pthread src/socket.server.c obj/tabla.o -o bin/server
	./bin/server

server.t: src/socket.server.c proc.o utils.o
	@gcc -Wall -Iinclude/ -pthread obj/utils.o obj/proc.o src/socket.server.c -o bin/server
	@./bin/server

client: src/socket.client.c
	gcc  -Wall -Wextra src/socket.client.c -o bin/client -pthread
	./bin/client test/dump/hello.c

prueba: src/prueba.c tabla.o parson.o
	gcc  -pthread -Iinclude/ -Iparson/ obj/tabla.o  obj/parson.o src/prueba.c  -o prueba
	./prueba

utils.test: src/utils.c utils.o
	@gcc  -pthread -Iinclude/ obj/utils.o test/utils.test.c  -o bin/utils.test
	@./bin/utils.test

proc.test: src/proc.c proc.o utils.o
	@gcc  -pthread -Iinclude/ obj/utils.o obj/proc.o test/proc.test.c  -o bin/proc.test
	@./bin/proc.test

clean:
	rm .data/* prueba obj/*.o

test: utils.test proc.test
