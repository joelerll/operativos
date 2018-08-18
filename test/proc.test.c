#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
int main() {
  ProcFile **cpus = proc_listaProcesos("./test/proc");
  int CPUs = sysconf(_SC_NPROCESSORS_ONLN);
  int contador = 0;
  ProcFile *cpu;
  do {
    cpu = cpus[contador];
    contador++;
  } while (cpu != NULL);
  assert((contador - 2) == CPUs);

  int procesador = proc_obtenerProcesosMenosUsado("./test/proc");
  assert(procesador == 8);

  return 0;
}
