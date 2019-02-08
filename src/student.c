#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "conf.h"
#include "student.h"
#include "group.h"
#include "random.h"
#include "debug.h"

extern double TWO;
extern double THREE;
extern double FOUR;

student* new_student() {
  student* s = malloc(sizeof(student));
  s->pid = 0;
  s->voto_AdE = random_int_range(18, 30);
  s->nof_elems = random_nof_elems(TWO, THREE, FOUR);
  s->vote = 0;
  s->group = NULL;
  s->invite = TRUE;
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

int student_imp(student* a, student* b) {
  int diff = abs(a->voto_AdE - b->voto_AdE);
  if (a->nof_elems == b->nof_elems)
    return diff;
  else return diff-3;
}

int student_imp_comp(void* a, void* b, void* self) {
  student** x = a;
  student** y = b;
  int imp0 = student_imp(*x,(student*)self);
  int imp1 = student_imp(*y,(student*)self);
  if (imp0 > imp1)
    return -1;
  else if (imp0 < imp1)
    return 1;
  else return 0;
}

int student_vote_comp(void* a, void* b) {
  student** x = a;
  student** y = b;
  int v1 = (*x)->voto_AdE;
  int v2 = (*y)->voto_AdE;
  if (v1 > v2)
    return -1;
  else if (v1 < v2)
    return 1;
  else {
    return 0;
  }
}


void print_student(void* obj) {
  student* s = (student*)obj;
  printf("{ pid: %d, voto_AdE: %u, nof_elems: %u, vote: %u, group: %p, invite: %d }", s->pid, s->voto_AdE, s->nof_elems, s->vote, s->group, s->invite);
}

void student_print_array(student** a, int size) {
  for (int i = 0; i < size;i++) {
    print_student(a[i]);
    printf("\n");
  }
}
