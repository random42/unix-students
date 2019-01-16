#ifndef SEM_H
#define SEM_H

#include "conf.h"

int sem_get(int key);
int sem_create(int key, int n_sems);
void sem_delete(int sem_id);
void sem_op(int sem_id, int sem_num, int op, bool wait);
void sem_set(int sem_id, short sem_num, short val);

#endif
