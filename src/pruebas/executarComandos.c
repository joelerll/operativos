// int status = system("ls");
FILE *ls = popen("ls -mesf 2>/dev/null", "r");
// popen("/bin/ls /etc/ 2>&1", "r");
char buf[256];
while (fgets(buf, sizeof(buf), ls) != 0) {
  printf("%s\n", buf);
}
pclose(ls);


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);

int main(void)
{
    FILE *cmd;
    char s[64];
    double cpu;

    sprintf(s, "ps --no-headers -p %d -o %%cpu", (int)getpid());
    cmd = popen(s, "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    if (fgets(s, sizeof(s), cmd) != NULL) {
        cpu = strtod(s, NULL);
        printf("%f\n", cpu);
    }
    pclose(cmd);
    return 0;
}
