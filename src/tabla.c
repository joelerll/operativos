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

tablaDatos *tabla_crear (char *userId, char *fechaEnvio, char *rutaFuente, char *respuesta, char *rutaEjecutable, int ordenLlegada) {
  tablaDatos *tabla = (tablaDatos *)malloc(sizeof(tablaDatos));
  tabla->userId = (char *)malloc(sizeof(char) * sizeof(userId));
  tabla->fechaEnvio = (char *)malloc(sizeof(char) * sizeof(fechaEnvio));
  tabla->rutaEjecutable = (char *)malloc(sizeof(char) * sizeof(rutaEjecutable));
  tabla->respuesta = (char *)malloc(sizeof(char) * sizeof(respuesta));
  tabla->rutaFuente = (char *)malloc(sizeof(char) * sizeof(rutaFuente));
  tabla->ordenLlegada = ordenLlegada;
  tabla->siguiente = NULL;
  tabla->anterior = NULL;
  strcpy(tabla->userId, userId);
  strcpy(tabla->fechaEnvio, fechaEnvio);
  strcpy(tabla->respuesta, respuesta);
  strcpy(tabla->rutaEjecutable, rutaEjecutable);
  strcpy(tabla->rutaFuente, rutaFuente);
  return tabla;
}

Tablas *tabla_master_crear () {
  Tablas *tabla = (Tablas *)malloc(sizeof(Tablas));
  tabla->datos = (tablaDatos *)malloc(sizeof(tablaDatos));
  tabla->cantidad = 0;
  return tabla;
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
    tablas->cantidad = tablas->cantidad + 1;
  }

}

tablaDatos *tabla_buscarPorUserId (char *userId, Tablas *tablas) {
  for (tablaDatos *i = tablas->datos; i != NULL; i = i->siguiente) {
    if (strcmp(userId, i->userId) == 0) {
      return i;
    }
  }
  return NULL;
}

void tabla_actualizar (tablaDatos *tabla, char *rutaEjecutable, char *respuesta) {
  tabla->respuesta = (char *)malloc(sizeof(char) * sizeof(respuesta));
  tabla->rutaEjecutable = (char *)malloc(sizeof(char) * sizeof(rutaEjecutable));
  strcpy(tabla->respuesta, respuesta);
  strcpy(tabla->rutaEjecutable, rutaEjecutable);
}
