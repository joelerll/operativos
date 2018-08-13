typedef struct tablaDatos {
  char *userId;
  char *fechaEnvio;
  char *respuesta;
  char *rutaFuente;
  char *rutaEjecutable;
  int ordenLlegada;
  struct tablaDatos *siguiente;
  struct tablaDatos *anterior;
}tablaDatos;

typedef struct {
  tablaDatos *datos;
  int cantidad;
}Tablas;

tablaDatos *tabla_crear  (char *userId, char *fechaEnvio, char *rutaFuente, char *respuesta, char *rutaEjecutable, int ordenLlegada);
Tablas *tabla_master_crear ();
void tabla_anadirTablaHead (Tablas *tablas, tablaDatos *tabla);
tablaDatos *tabla_buscarPorUserId (char *userId, Tablas *tablas);
void tabla_anadirTabla (Tablas *tablas, tablaDatos *tabla);
void tabla_actualizar (tablaDatos *tabla, char *rutaEjecutable, char *respuesta);