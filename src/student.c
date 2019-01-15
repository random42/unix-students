#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "student.h"
#include "conf.h"
#include "random.h"

extern config conf;

student self;

student* new_student(int pid) {
  student* s = malloc(sizeof(student));
  s->pid = pid;
  s->voto_AdE = random_uint_range(18, 30);
  s->nof_elems = random_nof_elems(&conf);
  s->vote = 0;
  s->group = NULL;
  return s;
}

uint potential_vote(student* a, student* b) {
  uint vote = a->voto_AdE;
  if (b->voto_AdE > vote)
    vote = b->voto_AdE;
  if (a->nof_elems != b->nof_elems)
    vote -= 3;
  return vote;
}

void print_student(void* obj) {
  student* s = (student*)obj;
  char* sep = "\n   ";
  printf("{%spid: %d,%svoto_AdE: %u,%snof_elems: %u,%svote: %u,%sgroup: %p\n}\n", sep, s->pid, sep, s->voto_AdE, sep, s->nof_elems, sep, s->vote, sep, s->group);
}
