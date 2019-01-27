#ifndef SHM_H

#define SHM_H

#include "list.h"

typedef struct {
  int pid;
  int voto_AdE;
  int nof_elems;
  bool in_closed_group;
} shm_student;

void* shm_create(int students);
void shm_write_students(list* students);
void shm_close_group(int* pids, int length);
void* shm_get();
void shm_read();
void shm_stop_read();
void shm_write();
void shm_stop_write();
void shm_delete();
void shm_print();

#endif
