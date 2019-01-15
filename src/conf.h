#ifndef CONF_H
#define CONF_H
#include <stdlib.h>
#define ERROR(x); printf(x);exit(1);

typedef unsigned int uint;
typedef enum {FALSE,TRUE} bool;

typedef struct {
  double two;
  double three;
  double four;
  uint nof_invites;
  uint max_reject;
} config;

void config_init();

#endif
