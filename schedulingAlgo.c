#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ScisSos.h"
#include "ScisSosAssign.h"

int sched_rnd(int qlen) {
    if (qlen <= 0) {
        fprintf(stderr, "No ready processes to schedule\n");
        return -1;
    }
    int random_index = rand() % qlen;
    int selected_pid = _readyQ[random_index];
    return selected_pid;
}

int sched_fifo(int qlen) {
    if (qlen <= 0) {
        fprintf(stderr, "No ready processes to schedule\n");
        return -1;
    }
    int selected_pid = _readyQ[0];
    return selected_pid;
}

// smallest size first
int sched_sml(int qlen) {
    if (qlen <= 0) {
        fprintf(stderr, "No ready processes to schedule\n");
        return -1;
    }
    int selected_pid = _readyQ[0];
    for (int i = 1; i < qlen; i++) {
        if (_proctable[_readyQ[i]]->size < _proctable[selected_pid]->size) {
            selected_pid = _readyQ[i];
        }
    }
    return selected_pid;
}

// smallest remaining time first
int sched_srt(int qlen) {
    if (qlen <= 0) {
        fprintf(stderr, "No ready processes to schedule\n");
        return -1;
    }
    int selected_pid = _readyQ[0];
    for (int i = 1; i < qlen; i++) {
        int current_pid = _readyQ[i];
        int selected_remaining_time = _proctable[selected_pid]->size - _proctable[selected_pid]->pc;
        int current_remaining_time = _proctable[current_pid]->size - _proctable[current_pid]->pc;
        if (current_remaining_time < selected_remaining_time) {
            selected_pid = current_pid;
        }
    }
    return selected_pid;
}