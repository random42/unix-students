#ifndef GROUP_H

#define GROUP_H
#include "conf.h"
#include "list.h"
#include "student.h"

typedef struct group_t {
  int size;
  student** students;
  student* leader;
  bool closed;
  int max_vote;
} group;

group* new_group();
void group_add_student(group* g, student* s);
void print_student_num(void* obj);
void print_group(void* obj);

#endif
