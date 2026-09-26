#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ScisSos.h"
#include "ScisSosAssign.h"

void _freePCB(ScisSosPCB* pcb) {
    if (pcb->p_code) {
        for (int i = 0; i < pcb->size; i++) {
            free(pcb->p_code[i]);
        }
        free(pcb->p_code);
    }
    free(pcb);
}

int lenReadyQ;
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pcount = atoi(argv[1]); // Number of processes to create
    if (pcount <= 0) {
        fprintf(stderr, "Invalid number of processes\n");
        exit(EXIT_FAILURE);
    }

    scissos_initialise();

    ScisSosProcess* proc = ScisSosPCreator(pcount);
    print_process_struct(proc, stdout);
    scissos_print_pcb(proc, stdout);
    free(proc);

    do {
        scissos_call_scheduler();
    } while (lenReadyQ > 0);

    scissos_print_timings();

    for (int i = 0; i < lenProcTable; i++) {
        if (_proctable[i]) {
            _freePCB(_proctable[i]);
        }
    }
    return 0;
}