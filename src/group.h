#ifndef GROUP_H

#define GROUP_H
#include "conf.h"
#include "student.h"

typedef struct group_t {
  uint size;
  list* students;
  student* leader;
} group;

group* new_group();
void add_student(group* g, student* s);
void print_student_num(void* obj);
void print_group(void* obj);
void set_votes(group*g);

#endif
