#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

// Δομή pstat για αποθήκευση πληροφοριών όλων των διεργασιών

struct pstat {
    char p_name[NPROC][16];     // Ονόματα
    char st_name[NPROC][10];    // Καταστάσεις
    int pid[NPROC];             // PIDs
    int ppid[NPROC];            // PIDs γονέων
    uint64 size[NPROC];         // Μεγέθοι
    int priority[NPROC];        // Προτεραιότητες (0-3)
    int ticks[NPROC];           // Ticks στην τρέχουσα προτεραιότητα
    int num_proc;               // Πλήθος διεργασιών
};


#endif // _PSTAT_H_