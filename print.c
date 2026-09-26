#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "ScisSos.h"
#include "ScisSosAssign.h"

int lenProcTable;
TIMING pr_times[MAXPROC];
ScisSosPCB* _proctable[MAXPROC];

void scissos_print_timings() {
    printf("Process Timing Information:\n");
    for (int i = 0; i < lenProcTable; i++) {
        ScisSosPCB* pcb = _proctable[i];
        TIMING* timing = &pr_times[pcb->pid];
        printf("PID:%d Creation: %ld.%06d Response: %ld.%06d Waiting: %ld.%06d Running: %ld.%06d \n",
            pcb->pid,
            timing->crt_time.tv_sec, timing->crt_time.tv_usec,
            timing->rsp_time.tv_sec, timing->rsp_time.tv_usec,
            timing->wt_time.tv_sec, timing->wt_time.tv_usec,
            timing->run_time.tv_sec, timing->run_time.tv_usec);
    }
}

void scissos_print_pcb(ScisSosProcess* proc, FILE* fp) {
    if (!proc || !proc->_pcb) {
        fprintf(fp, "Invalid process or PCB\n");
        return;
    }
    ScisSosPCB* pcb = proc->_pcb;
    fprintf(fp, "PCB Information:\n");
    fprintf(fp, "PID: %d\n", pcb->pid);
    fprintf(fp, "User ID: %d\n", pcb->uid);
    fprintf(fp, "Size: %d\n", pcb->size);
    fprintf(fp, "Priority Value: %d\n", pcb->priority_value);
    fprintf(fp, "Process State: %d\n", pcb->ps_state);
    fprintf(fp, "Process Type: %d\n", pcb->p_type);
    fprintf(fp, "Memory Behaviour: %d\n", pcb->m_type);
    fprintf(fp, "Program Counter: %d\n", pcb->pc);
    fprintf(fp, "Current Time-Slice: %d\n\n", pcb->p_timeslice);
}

void print_process_struct(ScisSosProcess* proc, FILE* fp) {
    if (!proc) {
        fprintf(fp, "Invalid process\n");
        return;
    }
    fprintf(fp, "Process Information:\n");
    fprintf(fp, "Process Name: %s\n", proc->_pname);
    fprintf(fp, "PID: %d\n", proc->_PID);
    fprintf(fp, "Size: %d\n", proc->_psize);
}