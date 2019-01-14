#ifndef CONF_H

#define CONF_H

#define error(x) printf(x);exit(EXIT_FAILURE);

typedef unsigned int uint;
typedef enum {FALSE,TRUE} bool;

typedef struct {
  double two;
  double three;
  double four;
  uint nof_invites;
} config;

#endif
