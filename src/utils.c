#include "utils.h"
#include <ctype.h> // isspace
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#define ESPACIO 32
#define SALTO_LINEA = 10
#define TRUE 1
#define FALSE 0
#define LIMITE_SUPERIOR_MAYUSCULAS 90
#define LIMITE_INFERIOR_MAYUSCULAS 65
#define LIMITE_SUPERIOR_MINUSCULAS 122
#define LIMITE_INFERIOR_MINUSCULAS 97

// deprecated
void u_trim(char * s) {
  char * p = s;
  int l = strlen(p);
  while(isspace(p[l - 1])) p[--l] = 0;
  while(* p && isspace(* p)) ++p, --l;
  memmove(s, p, l + 1);
}

void u_clear() { system("clear"); }

// deprecated
int u_id_generator(unsigned long u_id) {
  struct timeval t;
  unsigned long id;
  gettimeofday(&t,NULL);
  id = ((t.tv_sec * 1000 * 1000) + (t.tv_usec * 1000)) << 42;
  id |= (u_id % 16777216) << 24;
  return id;
}

char *u_human_readable(size_t size) {
	char *print = "%.1f %s";
  static const char *SIZES[] = { "B", "kB", "MB", "GB" };
  size_t div = 0;
  size_t rem = 0;

  while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
    rem = (size % 1024);
    div++;
    size /= 1024;
  }

	char *datos = malloc(sizeof(char *) * sizeof(print) * 2);
	snprintf(datos, 16, print, (float)size + (float)rem / 1024.0, SIZES[div]);
	return datos;
}

int u_random_number(int min_num, int max_num) {
  int result = 0, low_num = 0, hi_num = 0;
  if (min_num < max_num) {
    low_num = min_num;
    hi_num = max_num + 1;
  } else {
    low_num = max_num + 1;
    hi_num = min_num;
  }
  result = (rand() % (hi_num - low_num)) + low_num;
  return result;
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

void u_time_convert (double time) {
	// 	auto n=seconds;
	//
	// sec = n % sec_per_min;
	// n /= sec_per_min;
	//
	// min = n % min_per_hr;
	// n /= min_per_hr;
	//
	// hr = n % hr_per_day;
	// n /= hr_per_day;
	//
	// day = n;
}

void u_escribir_log (char *mensaje, char *nombreArchivo) {
	int fp = open(nombreArchivo, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if(fp < 0) {
    perror("Error opening file.");
	} else {
		write(fp, mensaje, strlen(mensaje));
		write(fp, "\n", strlen("\n"));
	}
	close(fp);
}

void u_limpiar_log (char *nombreArchivo) {
	FILE *fp = fopen(nombreArchivo,"w");
	if(fp == NULL) {
    perror("Error opening file.");
	}
	fclose(fp);
}

void u_delete_number (unsigned int *numeros, int tamano, int numero) {
	int *numeroTmp = malloc(sizeof(int) * tamano);
	int contadorReal = 0;
	// copiar los elementos
	for (size_t i = 0; i < tamano; i++) {
		numeroTmp[i] = numeros[i];
	}

	// encerar todos a 0
	memset(numeros, 0, tamano);

	// eliminar el numero
	for (size_t i = 0; i < tamano; i++) {
		if (numeroTmp[i] == numero) {
			continue;
		}
		numeros[contadorReal] = numeroTmp[i];
		contadorReal++;
	}
	free(numeroTmp);
}

void u_print_current_folder () {
	char cwd[500];
	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
}

char *u_gen_fecha () {
	char *fecha = malloc(sizeof(50));
	const char *userFormato = "%d/%d/%dH%d:%d:%d";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int year = tm.tm_year + 1900;
	int mon = tm.tm_mon + 1;
	int day = tm.tm_mday;
	int hora = tm.tm_hour;
	int minuto = tm.tm_min;
	int sec = tm.tm_sec;
	snprintf (fecha, 50, userFormato, year, mon, day, hora, minuto, sec);
	return fecha;
}
