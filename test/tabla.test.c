#include "stdio.h"
#include "tabla.h"
int main() {
  tablaDatos *tabla = tabla_crear ("userId", "fechaEnvio", "rutaFuente", "", "", 5);
  tablaDatos *tabla2 = tabla_crear ("userId2", "fechaEnvio2", "rutaFuente2", "", "", 5);
  Tablas *tablas = tabla_master_crear();
  tabla_anadirTabla (tablas, tabla);
  tabla_anadirTabla (tablas, tabla2);
  tablaDatos *a = tabla_buscarPorUserId ("userId2", tablas);
  if (a != NULL) {
    printf("%s\n", a->userId);
    printf("%s\n", a->fechaEnvio);
    printf("%s\n", a->rutaEjecutable);
    tabla_actualizar (a, "aaaa", "aaaa");
    printf("%s\n", a->rutaEjecutable);
  }
  return 0;
}