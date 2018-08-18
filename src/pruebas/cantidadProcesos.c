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

  /proc/cpuinfo
Information about the processor, such as its type, make, model, and performance.

/proc/[pid]/cmdline
This holds the complete command line for the process, unless the whole process has been swapped out, or unless the process is a zombie. In either of these later cases, there is nothing in this file: i.e. a read on this file will return 0 characters. The command line arguments appear in this file as a set of null-separated strings, with a further null byte after the last string.

/proc/[pid]/cwd
This is a link to the current working directory of the process.

/proc/[pid]/environ
This file contains the environment for the process. The entries are separated by null characters, and there may be a null character at the end.

/proc/[pid]/exe
The exe file is a symbolic link containing the actual path name of the executed command. The exe symbolic link can be dereferenced normally – attempting to open exe will open the executable. You can even type /proc/[pid]/exe to run another copy of the same process as [pid].

/proc/[pid]/fd
This is a subdirectory containing one entry for each file which the process has opened, named by its file descriptor, and which is a symbolic link to the actual file (as the exe entry does). Thus, 0 is standard input, 1 standard output, 2 standard error, etc.

/proc/[pid]/maps
A file containing the currently mapped memory regions and their access permissions.

/proc/[pid]/mem
The mem file provides a means to access the process memory pages, using open, fseek and read commands.

/proc/[pid]/root
This is a link to the root directory which is seen by the process. This root directory is usually “/”, but it can be changed by the chroot command.

/proc/[pid]/stat
This file provides status information about the process. This is used by the Process Show utility. It is defined in fs/proc/array.c source file and may differ from one distribution to another.

/proc/devices
List of device drivers configured into the currently running kernel.

/proc/dma
Shows which DMA channels are being used at the moment.

[root@devops ~]# cat /proc/dma
4: cascade

/proc/filesystems
Filesystems configured into the kernel.

[root@devops ~]# cat /proc/filesystems
nodev sysfs
nodev rootfs
nodev bdev
nodev proc
nodev cgroup
nodev cpuset
nodev tmpfs
nodev devtmpfs
nodev binfmt_misc
nodev debugfs
nodev securityfs
nodev sockfs
nodev usbfs
nodev pipefs
nodev anon_inodefs
nodev inotifyfs
nodev devpts
nodev ramfs
nodev hugetlbfs
iso9660
nodev pstore
nodev mqueue
ext3
nodev xenfs

/proc/interrupts
Shows which interrupts are in use, and how many of each there have been.

/proc/ioports
Which I/O ports are in use at the moment.

[root@devops ~]# cat /proc/ioports
0000-0cf7 : PCI Bus 0000:00
0000-001f : dma1
0020-0021 : pic1
0040-0043 : timer0
0050-0053 : timer1
0060-0060 : keyboard
0064-0064 : keyboard
0070-0071 : rtc0
0080-008f : dma page reg
00a0-00a1 : pic2
00c0-00df : dma2

/proc/kcore
An image of the physical memory of the system. This is exactly the same size as your physical memory, but does not really take up that much memory; it is generated on the fly as programs access it. (Remember: unless you copy it elsewhere, nothing under /proc takes up any disk space at all.)

/proc/kmsg
Messages output by the kernel. These are also routed to syslog.

/proc/ksyms
Symbol table for the kernel.

/proc/loadavg
The `load average’ of the system; three meaningless indicators of how much work the system has to do at the moment.

/proc/meminfo
Information about memory usage, both physical and swap.

[root@devops ~]# cat /proc/meminfo
MemTotal: 500832 kB
MemFree: 116376 kB
Buffers: 1964 kB
Cached: 38772 kB
SwapCached: 0 kB
Active: 157548 kB
Inactive: 187552 kB
Active(anon): 149852 kB
Inactive(anon): 154960 kB
Active(file): 7696 kB
Inactive(file): 32592 kB
Unevictable: 0 kB
Mlocked: 0 kB
SwapTotal: 0 kB
SwapFree: 0 kB
Dirty: 96 kB
Writeback: 0 kB
AnonPages: 304388 kB
Mapped: 11180 kB
Shmem: 448 kB
Slab: 25164 kB
SReclaimable: 5872 kB
SUnreclaim: 19292 kB
KernelStack: 1040 kB
PageTables: 4028 kB
NFS_Unstable: 0 kB
Bounce: 0 kB
WritebackTmp: 0 kB
CommitLimit: 250416 kB

....

/proc/modules
Which kernel modules are loaded at the moment.

[root@devops ~]# cat /proc/modules
ipv6 322541 40 - Live 0xffffffffa0112000
xenfs 6087 1 - Live 0xffffffffa010b000
dm_mod 81692 0 - Live 0xffffffffa00eb000
xen_netfront 18905 0 - Live 0xffffffffa00da000
i2c_piix4 12608 0 - Live 0xffffffffa00d1000
i2c_core 31276 1 i2c_piix4, Live 0xffffffffa00c1000
sg 30124 0 - Live 0xffffffffa00b2000
ext3 240636 2 - Live 0xffffffffa0064000
jbd 80433 1 ext3, Live 0xffffffffa0045000
mbcache 8144 1 ext3, Live 0xffffffffa003d000
sr_mod 16228 0 - Live 0xffffffffa0033000
cdrom 39803 1 sr_mod, Live 0xffffffffa0022000

/proc/net
Status information about network protocols.

/proc/self
A symbolic link to the process directory of the program that is looking at /proc. When two processes look at /proc, they get different links. This is mainly a convenience to make it easier for programs to get at their process directory.

/proc/stat
Various statistics about the system, such as the number o

/proc/uptime
The time the system has been up.

/proc/version
The kernel version.
