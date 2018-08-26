
#define MMAP_NAME         "/tmp/mmaptest"
#define LENGTH            sizeof(struct mapped)
#define ARRAY_ELEMENTS    10

struct mapped
{
  unsigned char    array[ARRAY_ELEMENTS];
  int              use_mutex;
  pthread_mutex_t  mutex;
};

void die(char *msg)
{
  perror(msg);
  exit(1);
}

void child(struct mapped *mapping)
{
  unsigned int n = 0;
  while (1)
  {
    if (mapping->use_mutex)
      pthread_mutex_lock(&mapping->mutex);
    for (int i = 0; i < 10; i ++)
    {
      mapping->array[i] = n;
    }
    n ++;
    if (mapping->use_mutex)
      pthread_mutex_unlock(&mapping->mutex);
  }
}

void parent(struct mapped *mapping)
{
  while (1)
  {
    if (mapping->use_mutex)
      pthread_mutex_lock(&mapping->mutex);
    for (int i = 0; i < 10; i ++)
    {
      printf("%4d", mapping->array[i]);
    }
    printf("\n");
    if (mapping->use_mutex)
      pthread_mutex_unlock(&mapping->mutex);
    sleep(1);
  }
}

int main(int argc, char **argv)
{
  int rc;
  unlink(MMAP_NAME);
  int fd = open(MMAP_NAME, O_CREAT|O_RDWR, 00600);
  if (fd == -1)
    die("open");
  rc = ftruncate(fd, LENGTH);
  if (rc != 0)
    die("ftruncate");

  struct mapped *mapping = (struct mapped *)mmap(NULL,
                                                 LENGTH,
                                                 PROT_READ|PROT_WRITE,
                                                 MAP_SHARED,
                                                 fd,
                                                 0);
  if (mapping == MAP_FAILED)
    die("mmap");
  close(fd);

  pthread_mutexattr_t mutexattr;
  rc = pthread_mutexattr_init(&mutexattr);
  if (rc != 0)
    die("pthread_mutexattr_init");
  rc = pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
  if (rc != 0)
    die("pthread_mutexattr_setpshared");
  pthread_mutex_init(&mapping->mutex, &mutexattr);
  if (rc != 0)
    die("pthread_mutex_init");

  if (argc >= 2
      && strcmp(argv[1], "--no-mutex") == 0)
    mapping->use_mutex = 0;
  else
    mapping->use_mutex = 1;

  switch (fork())
  {
    case -1:
      die("fork");

    case 0:
      child(mapping);
      break;

    default:
      parent(mapping);
      break;
  }

  rc = munmap(mapping, LENGTH);
  if (rc != 0)
    die("munmap");

  return 0;
}

// static int *glob_var = 0;
// int x = 0;
//
// typedef struct persona {
// 	int edad;
// } Persona;
//
// typedef struct
// {
//   bool done;
//   pthread_mutex_t mutex;
// } shared_data;
//
// static shared_data* data = NULL;
//
// void initialise_shared()
// {
//     // place our shared data in shared memory
//     int prot = PROT_READ | PROT_WRITE;
//     int flags = MAP_SHARED | MAP_ANONYMOUS;
//     data = mmap(NULL, sizeof(shared_data), prot, flags, -1, 0);
//     assert(data);
//
//     data->done = false;
//
//     // initialise mutex so it works properly in shared memory
//     pthread_mutexattr_t attr;
//     pthread_mutexattr_init(&attr);
//     pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
//     pthread_mutex_init(&data->mutex, &attr);
// }
//
// void run_child()
// {
//   while (true)
//   {
//       puts("child waiting. .. ");
//       usleep(500000);
//
//       pthread_mutex_lock(&data->mutex);
//       if (data->done) {
//           pthread_mutex_unlock(&data->mutex);
//           puts("got done!");
//           break;
//       }
//       pthread_mutex_unlock(&data->mutex);
//   }
//
//   puts("child exiting ..");
// }
//
// void run_parent(pid_t pid)
// {
//     puts("parent sleeping ..");
//     sleep(2);
//
//     puts("setting done ..");
//     pthread_mutex_lock(&data->mutex);
//     data->done = true;
//     pthread_mutex_unlock(&data->mutex);
//
//     waitpid(pid, NULL, NULL);
//
//     puts("parent exiting ..");
// }
//
// int main(int argc, char** argv)
// {
//     initialise_shared();
//
//     pid_t pid = fork();
//     if (!pid) {
//         run_child();
//     }
//     else {
//         run_parent(pid);
//     }
//
//     munmap(data, sizeof(data));
//     return 0;
// }

// int main() {
// 	// int procesos[500]; // (500 * 4)
// 	// procesos[0] = 1;
// 	// printf("%lu\n", sizeof(procesos));
//
//   int size = 100 * sizeof(int);
//   void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//   printf("Mapped at %p\n", addr);
//
//   int *shared = addr;
//   pid_t mychild = fork();
//   if (mychild > 0) {
//     shared[0] = 10;
//     shared[1] = 20;
//   } else {
// 		wait(NULL);
//     // sleep(1); // We will talk about synchronization later
//     printf("%d\n", shared[1] + shared[0]);
//   }
//
//   munmap(addr,size);
//   return 0;
// }
