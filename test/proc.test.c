#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
int main() {
  ProcFile **cpus = proc_listaProcesos("./test/proc", 5000);
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

	Uptime *uptime = proc_uptime("test/uptime");
	assert(uptime->uptime == 236544.440000);
	assert(uptime->idle == 422897.710000);

	Memory *mem = proc_read_mem(2555, "test/statm");
	assert(mem->totalProgramSize == 11318);
	assert(mem->redisentSetSize == 666);
	assert(mem->sharedPages == 489);
	assert(mem->text == 348);
	assert(mem->dataStack == 0);
	assert(mem->library == 178);
	assert(mem->dirtyPages == 0);


	IO *io = proc_read_io(7039, "test/io");
	assert(io->rchar == 8249156);
	assert(io->wchar == 9916);
	assert(io->syscr == 2961);
	assert(io->syscw == 279);
	assert(io->read_bytes == 196608);
	assert(io->write_bytes == 4096);
	assert(io->cancelled_write_bytes == 0);


	Procinfo *pinfo = proc_cpu(30033, "test/stat"); // raro: 30278
	double porcentaje = proc_cpu_procentaje(pinfo);
	printf("%u\n", pinfo->minflt);
	printf("%u\n", pinfo->majflt);
	// assert(porcentaje == 0.017118);
	// printf("%f\n", porcentaje);
  return 0;
}
