#include <stdio.h>
#include <sys/types.h>
typedef struct procTDA{
	char *nombre;
  // int user; // normal processes executing in user mode
  // int nice; // niced processes executing in user mode
  // int systemProc; // processes executing in kernel mode
  // int idle; // twiddling thumbs
  // int iowait; // waiting for I/O to complete
  // int irq; // servicing interrupts
  // int softirq; // servicing softirqs
  // int steal; // involuntary wait
  // int guest; // running a normal guest
  // int guest_nice; // running a niced guest
  int PrevIdle;
  int PrevNonIdle;
  int PrevTotal;
  int Idle;
  int NonIdle;
  int Total;
  double porcentajeUso;
}ProcFile;

typedef struct memoryTDA {
  long unsigned int totalProgramSize;
  int redisentSetSize;
  int sharedPages;
  int text;
  int dataStack;
  int library;
  int dirtyPages;
} Memory;

typedef struct ioTDA {
  long int rchar;
  long int wchar;
  long int syscr;
  long int syscw;
  long int read_bytes;
  long int write_bytes;
  long int cancelled_write_bytes;
} IO;

typedef struct uptimeTDA {
  double uptime;
  double idle;
} Uptime;

typedef struct statstruct_proc {
  int           pid;                      /** The process id. **/
  char          exName [50]; /** The filename of the executable **/
  char          state; /** 1 **/          /** R is running, S is sleeping,
			   D is sleeping in an uninterruptible wait,
			   Z is zombie, T is traced or stopped **/
  unsigned      euid,                      /** effective user id **/
                egid;                      /** effective group id */
  int           ppid;                     /** The pid of the parent. **/
  int           pgrp;                     /** The pgrp of the process. **/
  int           session;                  /** The session id of the process. **/
  int           tty;                      /** The tty the process uses **/
  int           tpgid;                    /** (too long) **/
  unsigned int	flags;                    /** The flags of the process. **/
  unsigned int	minflt;                   /** The number of minor faults **/
  unsigned int	cminflt;                  /** The number of minor faults with childs **/
  unsigned int	majflt;                   /** The number of major faults **/
  unsigned int  cmajflt;                  /** The number of major faults with childs **/
  int           utime;                    /** user mode jiffies **/
  int           stime;                    /** kernel mode jiffies **/
  int		cutime;                   /** user mode jiffies with childs **/
  int           cstime;                   /** kernel mode jiffies with childs **/
  int           counter;                  /** process's next timeslice **/
  int           priority;                 /** the standard nice value, plus fifteen **/
  unsigned int  timeout;                  /** The time in jiffies of the next timeout **/
  unsigned int  itrealvalue;              /** The time before the next SIGALRM is sent to the process **/
  int           starttime; /** 20 **/     /** Time the process started after system boot **/
  unsigned int  vsize;                    /** Virtual memory size **/
  unsigned int  rss;                      /** Resident Set Size **/
  long unsigned int  rlim;                     /** Current limit in bytes on the rss **/
  long unsigned int  startcode;                /** The address above which program text can run **/
  long unsigned int	endcode;                  /** The address below which program text can run **/
  long unsigned int  startstack;               /** The address of the start of the stack **/
  long unsigned int  kstkesp;                  /** The current value of ESP **/
  long unsigned int  kstkeip;                 /** The current value of EIP **/
  int		signal;                   /** The bitmap of pending signals **/
  int           blocked; /** 30 **/       /** The bitmap of blocked signals **/
  int           sigignore;                /** The bitmap of ignored signals **/
  int           sigcatch;                 /** The bitmap of catched signals **/
  unsigned int  wchan;  /** 33 **/        /** (too long) **/
  int		sched, 		  /** scheduler **/
                sched_priority;		  /** scheduler priority **/
} Procinfo;

ProcFile **proc_listaProcesos (char *archivo, long int tiempo);
int proc_obtenerProcesosMenosUsado (char *archivo);
void proc_imprimir_porcentajesCPUs (char *archivo);
Memory *proc_read_mem (pid_t pid, char *archivo);
IO *proc_read_io (pid_t pid, char *archivo);
Procinfo *proc_cpu (pid_t pid, char *archivo);
Uptime *proc_uptime (char *archivo);
double proc_cpu_procentaje (Procinfo *pinfo);
void proc_guardar_estaticticas_procesos (unsigned int pids[], int cantidad, char *nombreArchivo);
