ps -eo min_flt,maj_flt,cmd
http://blog.scoutapp.com/articles/2015/04/10/understanding-page-faults-and-memory-swap-in-outs-when-should-you-worry

ps axo %cpu,%mem,command,pid | sort -nr | head

/* rusage.c
    usage:
    rusage  [executable_file_or_script]
*/
#include <sys/types.h>
#include <stdlib.h>
#include <sys/resrouces.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

char cmd[ARG_MAX]={0x0};

int dump(void)
{
		struct rusage s;
		struct rusage *p=&s;

  	getrusage(RUSAGE_CHILDREN, p);
		fprintf(stdout, "maximum resident set size              =%ld\n", p->ru_maxrss);
		fprintf(stdout, "integral resident set size             =%ld\n", p->ru_idrss);
		fprintf(stdout, "page faults not requiring physical I/O =%ld\n", p->ru_minflt);
		fprintf(stdout, "page faults requiring physical I/O     =%ld\n", p->ru_majflt);
		fprintf(stdout, "swaps                                  =%ld\n", p->ru_nswap);
		fprintf(stdout, "block input operations                 =%ld\n", p->ru_inblock);
		fprintf(stdout, "block output operations                =%ld\n", p->ru_oublock);
		fprintf(stdout, "messages sent                          =%ld\n", p->ru_msgsnd);
		fprintf(stdout, "messages received                      =%ld\n", p->ru_msgrcv);
		fprintf(stdout, "signals received                       =%ld\n", p->ru_nsignals);
		fprintf(stdout, "voluntary context switches             =%ld\n", p->ru_nvcsw);
		fprintf(stdout, "involuntary context switches           =%ld\n", p->ru_nivcsw);
  	return 1;
}

int main(int argc, char *argv[])
{
    char *p=cmd;
    int i=1;
    ptrdiff_t d=ARG_MAX;

    for(i=1; i < argc; i++)
    {
        size_t len=strlen(argv[i]);
        if( d - len > ARG_MAX)
        {
           sprintf(p, "%s ", argv[i]);
           d-=len;
           p+=(len +1);
        }
        else
        {
        	errno=E2BIG;
        	perror("Invalid parameter list");
        	exit(1);
        }
    }
    if(*cmd)
    {
	  	system(cmd);
	  	dump();
	  }
	  else
	  {
	  	errno=EINVAL;
	  	perror("cannot execute command");
	  	exit(1);
	  }
    return 0;
}

#include
#include
#include


    int kResult = 0;
    struct rusage start, end;
    struct timeval timeS, timeE;
    double t= 0.0, time= 0.0;

    struct timespec st, endt;
    double t1= 0.0, time1= 0.0;

    // measure CPU time
      kResult = getrusage(RUSAGE_THREAD, &start);

        if (kResult == -1)
            fprintf(stderr, "\n\n Error in getrusage command");

        timeS = start.ru_stime; // system time
        t = (double)timeS.tv_sec + (double) timeS.tv_usec / 1000000.0;
        timeS = start.ru_utime; // user time
        t = t + (double)timeS.tv_sec + (double) timeS.tv_usec / 1000000.0;

        kResult = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &st);
        if (kResult == -1)
            fprintf(stderr, "\n\n Error in clock_gettime command");

        t1 = (double) (st.tv_sec * 1000000.0 )+ (double) st.tv_nsec / 1000.0 ;



   // Do some operation to consume CPU


  // measure CPU time

 kResult = getrusage(RUSAGE_THREAD, &end);

        if (kResult == -1)
            fprintf(stderr, "\n\n Error in getrusage command");

        timeE = end.ru_stime; // system time
        time = (double)timeE.tv_sec + (double) timeE.tv_usec / 1000000.0;
        timeE = end.ru_utime; // user time
        time = time + (double)timeE.tv_sec + (double) timeE.tv_usec / 1000000.0;

        time = time - t;
        fprintf(stderr,"\n\n Total CPU usage time using 'getrusage' = %.12lf\n\n", time);

        kResult = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &endt);
        if (kResult == -1)
            fprintf(stderr, "\n\n Error in clock_gettime command");

        time1 = (double) (endt.tv_sec * 1000000.0)+ (double) endt.tv_nsec / 1000.0 ;
        time1 = (time1 - t1) / 1000000.0;

        fprintf(stderr,"\n\n Total CPU usage time using 'clock_gettime' = %.12lf\n\n", time1);
