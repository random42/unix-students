#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "group.h"
#include "student.h"


group* new_group() {
  group* g = malloc(sizeof(group));
  g->students = new_list();
  g->leader = NULL;
  return g;
}

void add_student(group* g, student* s) {
  list_add(g->students, s);
  g->size++;
}

void print_student_num(void* obj) {
  student* s = (student*)obj;
  if (s)
    printf("%d",s->pid);
  else
    printf("NULL");
}

void print_group(void* obj) {
  group* g = (group*)obj;
  char* sep = "\n   ";
  printf("{%ssize: %u,%sstudents: ", sep, g->size, sep);
  print_list(g->students, print_student_num);
  printf(",%sleader: ", sep);
  print_student_num(g->leader);
  printf("\n}");
}

void set_votes(group* g) {
  if (!g->size || !g->leader)
    return;
  uint max_vote = 0;
  node* n = g->students->first;
  while (n) {
    student* s = (student*)n->elem;
    if (max_vote < s->voto_AdE)
      max_vote = s->voto_AdE;
    n = n->next;
  }
  n = g->students->first;
  while (n) {
    student* s = (student*)n->elem;
    if (s->nof_elems == g->size)
      s->vote = max_vote;
    else
      s->vote = max_vote - 3;
    n = n->next;
  }
}
