#ifndef SHM_H

#define SHM_H

#include "list.h"

void* shm_create();
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
