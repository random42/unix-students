#ifndef CONF_H
#define CONF_H
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#define ERROR(x, ...); printf("ERROR\n");printf(x, ##__VA_ARGS__);printf("%s\n",strerror(errno));exit(EXIT_FAILURE);
#define SEM_KEY 42435
#define START_SEM 0
#define TURN_SEM 1
#define DEBUG 1
#define debug(x, ...); if (DEBUG) {printf("%d ", getpid()); printf(x, ##__VA_ARGS__);}

typedef enum {FALSE,TRUE} bool;

void config_init();

#endif
