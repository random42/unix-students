#ifndef STUDENT_H

#define STUDENT_H
#include "conf.h"

struct group_t;

typedef struct {
  int pid; // also defines student number
  int voto_AdE;
  int nof_elems;
  int vote;
  struct group_t* group;
} student;


student* new_student(int pid);
int potential_vote(student* a, student* b);
void print_student(void* obj);

#endif
