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

student* new_student(pid_t pid);
uint potential_vote(student* me, student* b);

#endif
