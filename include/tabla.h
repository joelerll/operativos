#include <pthread.h>
typedef struct tablaDatos {
  // char *userId;
  int userId;
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
  pthread_mutex_t mutex;
}Tablas;

tablaDatos *tabla_crear(int userId, char *fechaEnvio, char *rutaFuente, char *respuesta, char *rutaEjecutable, int ordenLlegada);
void tabla_master_crear (Tablas *tabla);
void tabla_anadirTablaHead (Tablas *tablas, tablaDatos *tabla);
tablaDatos *tabla_buscarPorUserId (int userId, Tablas *tablas);
void tabla_anadirTabla (Tablas *tablas, tablaDatos *tabla);
void tabla_actualizar (tablaDatos *tabla, char *rutaEjecutable, char *respuesta);
void tabla_actualizar_ruta (tablaDatos *tabla, char *rutaEjecutable);
void tabla_actualizar_respuesta (tablaDatos *tabla, char *respuesta);
void tabla_imprimir (Tablas *tablas);
