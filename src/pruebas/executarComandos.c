// int status = system("ls");
FILE *ls = popen("ls -mesf 2>/dev/null", "r");
// popen("/bin/ls /etc/ 2>&1", "r");
char buf[256];
while (fgets(buf, sizeof(buf), ls) != 0) {
  printf("%s\n", buf);
}
pclose(ls);
