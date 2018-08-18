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
  return 0;
}
