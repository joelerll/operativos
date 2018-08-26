#include <sys/types.h> /* for size_t */

void u_trim(char * s);
char **u_split(char *palabras, char letra);
int u_random_number(int min_num, int max_num);
int u_id_generator(unsigned long u_id);
char *u_human_readable(size_t size);
void u_clear();
void u_delete_number (int *numeros, int tamano, int numero);
