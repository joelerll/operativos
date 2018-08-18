#include "utils.h"
#include <ctype.h> // isspace
#include <string.h>
#include <stdlib.h>
#define ESPACIO 32
#define SALTO_LINEA = 10
#define TRUE 1
#define FALSE 0
#define LIMITE_SUPERIOR_MAYUSCULAS 90
#define LIMITE_INFERIOR_MAYUSCULAS 65
#define LIMITE_SUPERIOR_MINUSCULAS 122
#define LIMITE_INFERIOR_MINUSCULAS 97

void u_trim(char * s) {
  char * p = s;
  int l = strlen(p);
  while(isspace(p[l - 1])) p[--l] = 0;
  while(* p && isspace(* p)) ++p, --l;
  memmove(s, p, l + 1);
}

char **u_split(char *palabras, char letra) {
  int cantidad = 0;
  for (int i = 0; i < strlen(palabras); ++i) {
    if (palabras[i] == letra)
      cantidad++;
  }
  char **palabrasSplit = malloc(sizeof(char *) * (cantidad + 1));
  int indexPalabraTmp = 0;
  int indexPalabrasTodas = 0;
  char palabraTmp[20];
  for (int i = 0; i < strlen(palabras); ++i) {
    if (palabras[i] == letra && strlen(palabraTmp) != 0) {
      char *palabraTmpNew = malloc(sizeof(char *));
      palabraTmp[indexPalabraTmp] = '\0';
      strcpy(palabraTmpNew, palabraTmp);
      palabrasSplit[indexPalabrasTodas] = palabraTmpNew;
      indexPalabrasTodas++;
      indexPalabraTmp = 0;
      memset(palabraTmp, 0, 20);
    } else {
      palabraTmp[indexPalabraTmp] = palabras[i];
      indexPalabraTmp++;
    }
    if (palabras[i + 1] == '\0') {
      char *palabraTmpNew = malloc(sizeof(char *));
      palabraTmp[indexPalabraTmp] = '\0';
      strcpy(palabraTmpNew, palabraTmp);
      palabrasSplit[indexPalabrasTodas] = palabraTmpNew;
      palabrasSplit[indexPalabrasTodas + 1] = NULL;
    }
  }
  return palabrasSplit;
}