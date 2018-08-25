#define _GNU_SOURCE
#define _BSD_SOURCE         /* See feature_test_macros(7) */
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <setjmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *glob_var = 0;
int x = 0;

typedef struct persona {
	int edad;
} Persona;

int main() {

  int size = 100 * sizeof(int);
  void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  printf("Mapped at %p\n", addr);

  int *shared = addr;
  pid_t mychild = fork();
  if (mychild > 0) {
    shared[0] = 10;
    shared[1] = 20;
  } else {
		wait(NULL);
    // sleep(1); // We will talk about synchronization later
    printf("%d\n", shared[1] + shared[0]);
  }

  munmap(addr,size);
  return 0;
}

// int main(void) {
//     glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE,
//                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//     *glob_var = 1;
//
//     if (fork() == 0) {
//         *glob_var = 5;
// 				printf("Hijo %d\n", x);
//         exit(EXIT_SUCCESS);
//     } else {
//         wait(NULL);
//         printf("%d\n", *glob_var);
// 				printf("Padre %d\n", x);
//         munmap(glob_var, sizeof *glob_var);
//     }
//     return 0;
// }
// int main(void) {
	// int pid = execl("/bin/ls", "/bin/ls", (char *)0);
	// printf("Pid: %d\n", pid);
// }



// pid_t child_pid, wpid;
// int status = 0;
//
// int n = 5;
// for (int id=0; id < n; id++) {
// 	if ((child_pid = fork()) == 0) {
// 			printf("Hijo %d\n", child_pid);
// 			exit(0);
// 	}
// }
//
// while ((wpid = wait(&status)) > 0);
