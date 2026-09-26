#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "ScisSos.h"
#include "ScisSosAssign.h"

struct timeval _diff_times(struct timeval end, struct timeval start) {
    struct timeval diff;
    if (end.tv_usec < start.tv_usec) {
        end.tv_sec--;
        end.tv_usec += 1000000;
    }
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_usec = end.tv_usec - start.tv_usec;

    return diff;
}
struct timeval _add_times(struct timeval a, struct timeval b) {

    struct timeval sum;

    sum.tv_sec = a.tv_sec + b.tv_sec;
    sum.tv_usec = a.tv_usec + b.tv_usec;

    if (sum.tv_usec >= 1000000) {
        sum.tv_sec++;
        sum.tv_usec -= 1000000;
    }
    return sum;
}

struct timeval _max_times(struct timeval a, struct timeval b) {
    struct timeval max_time;
    if (a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_usec > b.tv_usec)) {
        max_time = a;
    } else {
        max_time = b;
    }
    return max_time;
}