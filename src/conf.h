#ifndef CONF_H
#define CONF_H

#define SEM_KEY 42435
#define DEBUG_KEY 8383
#define START_SEM 0
#define TURN_SEM 1

//extern void sem_op(int sem_id, int sem_num, short op, int wait);

typedef enum {FALSE,TRUE} bool;

void config_init();

#endif
