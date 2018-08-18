#include <stdio.h>
#include <sys/sysinfo.h> // get_nprocs_conf get_nprocs
#include <unistd.h> // sysconf(_SC_NPROCESSORS_ONLN)
int main(int argc, char *argv[]) {
    printf("This system has %d processors configured and "
        "%d processors available.\n",
        get_nprocs_conf(), get_nprocs());
    int x = sysconf(_SC_NPROCESSORS_ONLN);
    printf("%d", x);
    return 0;
}

// while ((read = getline(&line, &len, fp)) != -1) {
//   // printf("Retrieved line of length %zu :\n", read);
//   // char **p = split(line, ' ');
//   // printf("%s\n", p[0]);
//   // char * nombre = parsear(line, cantidadCPU, procs);
//   char **procArray = parsearProc(line);
//   // if (nombre) {
//   //   printf("%s\n", nombre);
//   //   // printf("Cantidad 2: %s\n", procs[0]->user);
//   // }
//   // cantidadCPU++;
//   // printf("%s", line);
// }
// printf("Cantidad 2: %s\n", procs[1]->user);
// ProcFile *pr = procs[0];
// printf("%s\n", procs[0]->user);
// imprimir(pr);
// for(ProcFile i = procs[0]; i != null; i = procs++){
// 	mostrarInfoPersona(listaPersona[i]);
// 	printf("\n");
// }
// if (fp) {
//   while ((c = getc(fp)) != EOF) {
//     putchar(c);
//   }
//   fclose(fp);
// }

// fp = fopen("/etc/motd", "r");
// if (fp == NULL)
//     exit(EXIT_FAILURE);
//
// while ((read = getline(&line, &len, fp)) != -1) {
//     printf("Retrieved line of length %zu :\n", read);
//     printf("%s", line);
// }

  // for(;;)
  // {
  //     // cat /proc/stat
  //     fp = fopen("/proc/stat","r");
  //     fscanf(fp,"%*s %Lf %Lf %Lf %Lf\n",&a[0],&a[1],&a[2],&a[3]);
  //     fscanf(fp,"%*s %Lf %Lf %Lf %Lf\n",&c[0],&c[1],&c[2],&c[3]);
  //     fclose(fp);
  //     sleep(1);
  //
  //     fp = fopen("/proc/stat","r");
  //     fscanf(fp,"%*s %Lf %Lf %Lf %Lf\n",&b[0],&b[1],&b[2],&b[3]);
  //     fscanf(fp,"%*s %Lf %Lf %Lf %Lf\n",&d[0],&d[1],&d[2],&d[3]);
  //     fclose(fp);
  //
  //     loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
  //     load2 = ((d[0]+d[1]+d[2]) - (c[0]+c[1]+c[2])) / ((d[0]+d[1]+d[2]+d[3]) - (c[0]+c[1]+c[2]+c[3]));
  //     printf("The current CPU utilization is : %Lf\n",loadavg);
  //     printf("The current CPU utilization is : %Lf\n",load2);
  // }

  //
  //
  // #include <stdio.h>
  // #include <stdlib.h>
  // #include <sys/types.h>
  // #include <sys/stat.h>
  // #include <fcntl.h>
  // #include <unistd.h>
  //
  //
  // int *parser_result(const char *buf, int size){
  // 	static int ret[10];
  // 	int i, j = 0, start = 0;
  //
  // 	for(i=0; i<size; i++){
  // 		char c = buf[i];
  // 		if(c >= '0' && c <= '9'){
  // 			if(!start){
  // 				start = 1;
  // 				ret[j] = c-'0';
  // 			} else {
  // 				ret[j] *= 10;
  // 				ret[j] += c-'0';
  // 			}
  // 		} else if(c == '\n'){
  // 			break;
  // 		} else {
  // 			if(start){
  // 				j++;
  // 				start = 0;
  // 			}
  // 		}
  // 	}
  //
  // 	return ret;
  // }
  //
  //
  //
  // int main(int argc, char **argv){
  // 	char buf[256];
  // 	int size, fd, *nums, prev_idle = 0, prev_total = 0, idle, total, i;
  //
  // 	fd = open("/proc/stat", O_RDONLY);
  //
  // 	while(1){
  // 		size = read(fd, buf, sizeof(buf));
  // 		if(size <= 0)
  // 			break;
  //
  // 		nums = parser_result(buf, size);
  //
  // 		idle=nums[3];
  //
  // 		for(i=0, total=0; i<10; i++){
  // 			total += nums[i];
  // 		}
  //
  //
  // 		int diff_idle = idle-prev_idle;
  // 		int diff_total = total-prev_total;
  // 		float usage = (float)(((float)(1000*(diff_total-diff_idle))/(float)diff_total+5)/(float)10);
  // 		printf("\r%%%6.2f", usage);
  // 		fflush(stdout);
  //
  // 		prev_total = total;
  // 		prev_idle = idle;
  //
  // 		sleep(3);
  // 		lseek(fd, 0, SEEK_SET);
  // 	}
  //
  // 	return 0;
  // }
