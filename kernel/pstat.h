#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
    char p_name[NPROC][16];
    char st_name[NPROC][10]; 
    int pid[NPROC];   // PID of each process
    int ppid[NPROC];
    uint64 size[NPROC];
    int priority[NPROC]; // current priority level of each process (0-3)
    int ticks[NPROC]; // number of ticks each process has accumulated at each of 4 priorities
    int num_proc;
};


#endif // _PSTAT_H_