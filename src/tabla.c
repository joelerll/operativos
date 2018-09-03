#include "tabla.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *randstring(size_t length) {
  static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char *randomString = NULL;
  if (length) {
    randomString = malloc(sizeof(char) * (length +1));
    if (randomString) {
      for (int n = 0;n < length;n++) {
        int key = rand() % (int)(sizeof(charset) -1);
        randomString[n] = charset[key];
      }
      randomString[length] = '\0';
    }
  }
  return randomString;
}

tablaDatos *tabla_crear (int userId, char *fechaEnvio, char *rutaFuente, char *respuesta, char *rutaEjecutable, int ordenLlegada) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int year = tm.tm_year + 1900;
	int mon = tm.tm_mon + 1;
	int day = tm.tm_mday;
	int minuto = tm.tm_min;
	int hora = tm.tm_hour;
	int sec = tm.tm_sec;
	char *userFormato = "%d/%d/%dH%d:%d:%d";
  tablaDatos *tabla = (tablaDatos *)malloc(sizeof(tablaDatos));
	tabla->userId = userId;
  tabla->fechaEnvio = malloc(sizeof(char) * 50);
	snprintf(tabla->fechaEnvio, 50, userFormato, year, mon, day, hora, minuto, sec);
	tabla->rutaEjecutable = NULL;
	tabla->rutaFuente = malloc(sizeof(char *) * strlen(rutaFuente));
  tabla->respuesta = NULL;
  tabla->ordenLlegada = ordenLlegada;
  tabla->siguiente = NULL;
  tabla->anterior = NULL;
  strcpy(tabla->rutaFuente, rutaFuente);
  return tabla;
}

void tabla_master_crear (Tablas *tabla) {
  tabla = (Tablas *)malloc(sizeof(Tablas));
  tabla->datos = (tablaDatos *)malloc(sizeof(tablaDatos));
  tabla->cantidad = 0;
}

void tabla_anadirTablaHead (Tablas *tablas, tablaDatos *tabla) {
  tablas->datos = tabla;
  tablas->cantidad = 1;
}

void tabla_anadirTabla (Tablas *tablas, tablaDatos *tabla) {
  if (tablas->cantidad == 0) {
    tablas->datos = tabla;
    tablas->cantidad = 1;
  } else {
    tablaDatos *ultimo = NULL;
    for (tablaDatos *i = tablas->datos; i != NULL; i = i->siguiente) {
      ultimo = i;
    }
    ultimo->siguiente = tabla;
		tabla->siguiente = NULL;
    tablas->cantidad = tablas->cantidad + 1;
  }
}

tablaDatos *tabla_buscarPorUserId (int userId, Tablas *tablas) {
  for (tablaDatos *i = tablas->datos; i != NULL; i = i->siguiente) {
    // if (strcmp(userId, i->userId) == 0) {
    //   return i;
    // }
		if (userId == i->userId) {
      return i;
    }
  }
  return NULL;
}

void tabla_imprimir (Tablas *tablas) {
	printf("D: %d\n", tablas->cantidad);
	for (tablaDatos *i = tablas->datos; i != NULL; i = i->siguiente) {
		printf("Fecha: %s\n", i->rutaFuente);
	}
	printf("===\n");
}

void tabla_actualizar (tablaDatos *tabla, char *rutaEjecutable, char *respuesta) {
  tabla->respuesta = (char *)malloc(sizeof(char) * sizeof(respuesta));
  tabla->rutaEjecutable = (char *)malloc(sizeof(char) * strlen(rutaEjecutable));
  strcpy(tabla->respuesta, respuesta);
  strcpy(tabla->rutaEjecutable, rutaEjecutable);
}

void tabla_actualizar_ruta (tablaDatos *tabla, char *rutaEjecutable) {
  tabla->rutaEjecutable = (char *)malloc(sizeof(char *) * strlen(rutaEjecutable));
  strcpy(tabla->rutaEjecutable, rutaEjecutable);
}

void tabla_actualizar_respuesta (tablaDatos *tabla, char *respuesta) {
  tabla->respuesta = (char *)malloc(sizeof(char *) * strlen(respuesta));
  strcpy(tabla->respuesta, respuesta);
}
