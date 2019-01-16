#ifndef CONF_H
#define CONF_H
#include <stdlib.h>
#include <string.h>
#define ERROR(x); printf(x);printf("%s\n",strerror(errno));exit(1);
#define START_SEM_KEY 42435 // start sem

typedef enum {FALSE,TRUE} bool;

typedef struct {
  double two;
  double three;
  double four;
  int nof_invites;
  int max_reject;
} config;

void config_init();

#endif
