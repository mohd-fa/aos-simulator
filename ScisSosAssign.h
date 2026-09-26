#define MAXPROCSIZE 50000
#define MINPROCSIZE 24000

extern int lenProcTable;
extern int lenReadyQ;

int sched_rnd(int qlen);
int sched_fifo(int qlen);
int sched_sml(int qlen);
int sched_srt(int qlen);

void print_process_struct(ScisSosProcess*, FILE*);
