#include <stdlib.h>
#include <stdio.h>
#include "conf.h"


uint nof_elems,
nof_invites,
max_reject,
voto_AdE,
voto;

group g;

student* new_student(pid_t pid, FILE* random) {
  student* s = malloc(sizeof(student));
  s->pid = pid;
  fread(&s->matricola, sizeof(uint), 1, source);
  uint voto;
  fread(&voto, sizeof(uint), 1, source);
  s->voto_AdE = (voto % (30-18)) + 18;
}

void print_student(void* obj) {
  student* s = (student*)obj;

}
