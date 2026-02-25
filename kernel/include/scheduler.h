#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <process.h>

void scheduler_init(void);
void schedule(void);

extern process_table_t ptable;

#endif
