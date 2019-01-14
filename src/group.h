#ifndef GROUP_H

#define GROUP_H

typedef struct {
  uint size;
  student** students;
  student* leader;
} group;

group* new_group();
void add_student(group* g, student* s);

#endif
