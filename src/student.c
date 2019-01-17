#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "conf.h"
#include "student.h"
#include "group.h"
#include "random.h"

extern config conf;

student* new_student() {
  student* s = malloc(sizeof(student));
  s->pid = 0;
  s->voto_AdE = random_int_range(18, 30);
  s->nof_elems = random_nof_elems(&conf);
  s->vote = 0;
  s->group = NULL;
  return s;
}

int set_vote(student* s) {
  if (!s->group || !s->group->closed)
    s->vote = 0;
  else if (s->group->size == s->nof_elems)
    s->vote = s->group->max_vote;
  else
    s->vote = s->group->max_vote - 3;
  return s->vote;
}

void print_student(void* obj) {
  student* s = (student*)obj;
  char* sep = "\n   ";
  printf("{%spid: %d,%svoto_AdE: %u,%snof_elems: %u,%svote: %u,%sgroup: %p\n}\n", sep, s->pid, sep, s->voto_AdE, sep, s->nof_elems, sep, s->vote, sep, s->group);
}
