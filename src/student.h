#ifndef STUDENT_H

#define STUDENT_H

struct group;

typedef struct {
  int pid;
  uint matricola;
  uint voto_AdE;
  uint nof_elems;
  uint vote;
  struct group* group;
} student;


student* new_student(int pid, FILE* random, config* conf);
uint potential_vote(student* me, student* b);
void print_student(void* obj);

#endif
