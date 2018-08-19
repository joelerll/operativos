#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
int main() {

  // split
  char *casas = "casa casota  casita";
  char **casasSplit = u_split(casas, ' ');
  int contador = 0;
  char *palabra;
  do {
    palabra = casasSplit[contador];
    contador++;
  } while (palabra != NULL);
  assert((contador - 1) == 3);

  // id generator
  int id = u_id_generator(1);
  printf("%d\n", id);

	// human redeable
	char *tamano = u_human_readable(1024);
	assert(strcmp("1.0 kB", tamano) == 0);
	memset(tamano, 0, strlen(tamano));
	tamano = u_human_readable(2040);
	assert(strcmp("2.0 kB", tamano) == 0);
  return 0;
}
