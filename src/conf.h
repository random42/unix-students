#ifndef CONF_H
#define CONF_H
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define ERROR(x); printf("ERROR\n");printf(x);printf("%s\n",strerror(errno));raise(SIGTERM);
#define SEM_KEY 42435
#define START_SEM 0
#define TURN_SEM 1
#define DEBUG 1
#define debug(x); if (DEBUG) printf(x);

typedef enum {FALSE,TRUE} bool;

void config_init();

#endif
