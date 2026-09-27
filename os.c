#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ScisSos.h"
#include "ScisSosAssign.h"
int lenReadyQ;
void scissos_call_scheduler() {
    lenReadyQ = 0;
    for (int i = 0; i < lenProcTable; i++) {
        if (_proctable[i]->ps_state == PS_RUN) {
            _proctable[i]->ps_state = PS_RDY;
            printf("<- Process %d has been preempted and moved back to the ready queue\n", i);
        }
        if (_proctable[i]->ps_state == PS_RDY) {
            _readyQ[lenReadyQ++] = i;
        }
    }
    if (lenReadyQ == 0) {
        fprintf(stderr, "No ready processes to schedule\n");
        return;
    }
    int selected_pid = SCHEDFUNC(lenReadyQ);
    if (selected_pid == -1) {
        fprintf(stderr, "Failed to select a process to schedule\n");
        return;
    }
    _proctable[selected_pid]->ps_state = PS_RUN;
    printf("-> Process %d has been selected to run\n", selected_pid);
    scissos_proc_run(selected_pid);
}

void scissos_initialise() {
    int lenProcTable = 0;
    int lenReadyQ = 0;
    srand(time(NULL));
    gettimeofday(&_basetime, NULL);
    for(int i = 0; i < MAXPROC; i++) {
        _proctable[i] = NULL;
        _readyQ[i] = -1;
        pr_times[i].crt_time.tv_sec = 0;
        pr_times[i].crt_time.tv_usec = 0;
        pr_times[i].rsp_time.tv_sec = 0;
        pr_times[i].rsp_time.tv_usec = 0;
        pr_times[i].wt_time.tv_sec = 0;
        pr_times[i].wt_time.tv_usec = 0;
        pr_times[i].run_time.tv_sec = 0;
        pr_times[i].run_time.tv_usec = 0;
    }

}