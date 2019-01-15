#ifndef SHM_H

#define SHM_H
#define SHM_KEY 1234
#define SHM_SEM_KEY 1253
#include "conf.h"

typedef struct {
  int pid;
  uint voto_AdE;
  uint nof_elems;
  bool in_closed_group;
} shm_student;

void shm_init();
