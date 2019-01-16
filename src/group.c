#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "list.h"
#include "group.h"
#include "student.h"


group* new_group() {
  group* g = malloc(sizeof(group));
  g->students = new_list();
  g->leader = NULL;
  g->closed = FALSE;
  g->max_vote = 0;
  return g;
}

void add_student(group* g, student* s) {
  s->group = g;
  list_add(g->students, s);
  g->size++;
  if (s->voto_AdE > g->max_vote)
    g->max_vote = s->voto_AdE;
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
  printf(",%sclosed: %s,%s", sep, g->closed ? "TRUE" : "FALSE",sep);
  printf("max_vote: %d", g->max_vote);
  printf("\n}");
}
