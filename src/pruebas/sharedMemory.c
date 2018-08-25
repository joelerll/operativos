ftok() = generar un key unico
shmget() = returns an identifier for the shared memory segment. Crear el memory segmente
shmat() = attach the segment
shmdt() = detach shared memory
shm_open()
shm_unlink()
mmap()
munmap ()

// https://stackoverflow.com/questions/13274786/how-to-share-memory-between-process-fork

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *glob_var;



int main(void)
{
    glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *glob_var = 1;

    if (fork() == 0) {
        *glob_var = 5;
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
        printf("%d\n", *glob_var);
        munmap(glob_var, sizeof *glob_var);
    }
    return 0;
}
