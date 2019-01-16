#ifndef SHM_H

#define SHM_H

typedef struct {
  int pid;
  int voto_AdE;
  int nof_elems;
  bool in_closed_group;
} shm_student;

void* shm_create(int size);
void* shm_get();
void shm_read();
void shm_stop_read();
void shm_write();
void shm_stop_write();
void shm_delete();
