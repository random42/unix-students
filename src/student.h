#ifndef STUDENT_H

#define STUDENT_H
#include "conf.h"

struct group_t;

typedef struct {
  int pid; // also defines student number
  uint voto_AdE;
  uint nof_elems;
  uint vote;
  struct group_t* group;
} student;


student* new_student(int pid);
uint potential_vote(student* a, student* b);
void print_student(void* obj);

#endif
