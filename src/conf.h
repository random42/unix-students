#ifndef CONF_H
#define CONF_H
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define ERROR(x); printf("ERROR\n");printf(x);printf("%s\n",strerror(errno));raise(SIGTERM);
#define START_SEM_KEY 42435 // start sem
#define DEBUG 1
#define debug(x); if (DEBUG) printf("%s\n",x);

typedef enum {FALSE,TRUE} bool;

void config_init();

#endif
