#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "ScisSos.h"
#include "ScisSosAssign.h"

struct timeval _basetime;
TIMING pr_times[MAXPROC];
ScisSosPCB* _proctable[MAXPROC];
int _readyQ[MAXPROC];
int _blockQ[MAXPROC];
int lenProcTable;

ScisSosInst** _code_create(int psize, int ptype) {
    ScisSosInst** code;
    code = (ScisSosInst**)malloc(sizeof(ScisSosInst*) * psize);
    if (!code) {
        fprintf(stderr, "Memory allocation for code failed\n");
        exit(EXIT_FAILURE);
    }

    float threshold = ptype == PT_REG ? REG_THR * 10000 : (ptype == PT_CMP ? CMP_THR * 10000 : IOE_THR * 10000);

    for (int i = 0; i < psize; i++) {
        int random_value = rand() % 10000;
        code[i] = (ScisSosInst*)malloc(sizeof(ScisSosInst));
        code[i]->_inum = i;
        code[i]->_syscall = (random_value < threshold) ? INS_LNG : INS_SHR;
        code[i]->_addref = -1; // Initialize to -1 for future implementation
        if (!code[i]) {
            fprintf(stderr, "Memory allocation for instruction failed\n");
            exit(EXIT_FAILURE);
        }
    }
    return code;
}

ScisSosPCB* _pcb_create(int pid, int psize, int ptype, int mtype) {
    ScisSosPCB* pcb = (ScisSosPCB*)malloc(sizeof(ScisSosPCB));
    if (!pcb) {
        fprintf(stderr, "Memory allocation for PCB failed\n");
        exit(EXIT_FAILURE);
    }

    pcb->pid = pid; // PID will be assigned later
    pcb->uid = 1; // Default user ID
    pcb->size = psize;
    pcb->priority_value = DEFPRIO;
    pcb->ps_state = PS_NEW;
    pcb->p_type = ptype;
    pcb->m_type = mtype;
    pcb->pc = 0; // Program counter starts at 0
    pcb->p_code = NULL; // Will be assigned later
    // TODO:   Initialize page table entries
    pcb->p_timeslice = 0;
    return pcb;
}

ScisSosProcess* scissos_proc_create(char* pname, int psize, int ptype, int mtype) {
    ScisSosProcess* proc;
    int pid = lenProcTable;

    proc = (ScisSosProcess*)malloc(sizeof(ScisSosProcess));

    if (!proc) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize process structure
    strncpy(proc->_pname, pname, MAXPROCNAME);
    proc->_PID = pid; // PID will be assigned later
    proc->_psize = psize;
    proc->_pcb = _pcb_create(pid, psize, ptype, mtype);
    proc->_CODE = _code_create(psize, ptype);

    proc->_pcb->p_code = proc->_CODE;

    _proctable[pid] = proc->_pcb; // Store PCB in process table
    lenProcTable++;
    proc->_pcb->ps_state = PS_RDY;

    // Initialize timing information
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    pr_times[pid].crt_time = _diff_times(current_time, _basetime);
    pr_times[pid].wt_time.tv_sec = 0;
    pr_times[pid].wt_time.tv_usec = 0;
    pr_times[pid].run_time.tv_sec = 0;
    pr_times[pid].run_time.tv_usec = 0;

    return proc;
}

ScisSosProcess* ScisSosPCreator(int pcount) {
    // Make ScispCreator as a process
    ScisSosProcess* proc = (ScisSosProcess*)malloc(sizeof(ScisSosProcess));
    if (!proc) {
        fprintf(stderr, "Memory allocation failed for process creator\n");
        exit(EXIT_FAILURE);
    }
    int pid = lenProcTable;
    proc->_PID = lenProcTable;
    proc->_psize = pcount;
    strcpy(proc->_pname, "ScisSosPCreator");
    proc->_CODE = NULL;
    proc->_pcb = _pcb_create(pid, pcount, PT_CMP, 0);

    _proctable[pid] = proc->_pcb; // Store PCB in process table
    lenProcTable++;
    proc->_pcb->ps_state = PS_RDY;
    return proc;
}

void _runPCreator(int pid) {
    int pcount = _proctable[pid]->size;
    while (_proctable[pid]->pc < pcount) {
        char pname[MAXPROCNAME];
        snprintf(pname, MAXPROCNAME, "Process_%d", _proctable[pid]->pc + 1);

        int psize = (rand() % (MAXPROCSIZE - MINPROCSIZE + 1)) + MINPROCSIZE;
        int ptype = rand() % 3; // 0 for PT_REG, 1 for PT_CMP, 2 for PT_IOE;
        int mtype = 0; // Default memory type

        ScisSosProcess* proc = scissos_proc_create(pname, psize, ptype, mtype);
        print_process_struct(proc, stdout);
        scissos_print_pcb(proc, stdout);
        _proctable[pid]->pc++;
        free(proc);
    }
}

int scissos_proc_run(int pid) {
    if (pid < 0 || pid >= lenProcTable) {
        fprintf(stderr, "Invalid PID: %d\n", pid);
        return EXIT_FAILURE;
    }

    struct timeval start_time, current_turnaround_time, end_time;
    ScisSosPCB* pcb = _proctable[pid];

    gettimeofday(&start_time, NULL);
    start_time = _diff_times(start_time, _basetime);
    if (pcb->pc == 0)
        pr_times[pid].rsp_time = start_time;
    pcb->p_timeslice = 0;

    if (pid == 0) {
        _runPCreator(pid);
    } else {
        while (pcb->pc < pcb->size && pcb->p_timeslice < DEFTS) {
            ScisSosInst* inst = pcb->p_code[pcb->pc++];
            if (inst->_syscall == INS_LNG) {
                break;
            }
            pcb->p_timeslice++;
        }
    }

    gettimeofday(&end_time, NULL);
    end_time = _diff_times(end_time, _basetime);

    current_turnaround_time = _diff_times(end_time, start_time);

    pr_times[pid].run_time = _add_times(
        pr_times[pid].run_time,
        current_turnaround_time);

    pr_times[pid].wt_time = _diff_times(_diff_times(end_time, pr_times[pid].crt_time), pr_times[pid].run_time);

    if (pcb->pc >= pcb->size) {
        pcb->ps_state = PS_DEAD;
        printf("XX Process %d has completed execution and is now dead\n", pid);
    }
    return EXIT_SUCCESS;
}
