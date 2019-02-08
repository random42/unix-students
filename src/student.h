#ifndef STUDENT_H

#define STUDENT_H
#include "conf.h"

struct group_t;

typedef struct {
  int pid; // also defines student number
  int voto_AdE;
  int nof_elems;
  int vote;
  bool invite; // TRUE if student can be invited
  struct group_t* group;
} student;


student* new_student();
int set_vote(student* s);
void print_student(void* obj);
// overall improvement if they stay in the same group
int student_imp(student* a, student* b);
// compares a and b by improvement(a,self) and improvement(b,self);
int student_imp_comp(void* a, void* b, void* self);
int student_vote_comp(void* a, void* b);
void student_print_array(student** a, int size);

#endif
