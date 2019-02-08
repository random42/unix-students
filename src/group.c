#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "debug.h"
#include "list.h"
#include "group.h"
#include "student.h"


group* new_group() {
  group* g = malloc(sizeof(group));
  g->students = malloc(sizeof(student*) * 4);
  g->leader = NULL;
  g->closed = FALSE;
  g->max_vote = 0;
  return g;
}

void group_add_student(group* g, student* s) {
  if (g->size == 4) {
    error("Adding student to group of 4 students.");
  }
  s->group = g;
  g->students[g->size++] = s;
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
  printf("{%ssize: %u,%sstudents: [", sep, g->size, sep);
  for (int i = 0; i < g->size;i++) {
    print_student_num(g->students[i]);
    if (i < g->size-1) printf(",");
  }
  printf("],%sleader: ", sep);
  print_student_num(g->leader);
  printf(",%sclosed: %s,%s", sep, g->closed ? "TRUE" : "FALSE",sep);
  printf("max_vote: %d", g->max_vote);
  printf("\n}\n");
}
