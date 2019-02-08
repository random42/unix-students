#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "conf.h"
#include "shm.h"
#include "sem.h"
#include "list.h"
#include "student.h"
#include "group.h"
#include "debug.h"

#define SHM_KEY 1234
#define SHM_SEM_KEY 1253
#define READ 0 // num of readers
#define WRITE 1 // 0 if writing, 1 otherwise
#define WRITE_WAIT 2 // number of writers waiting

extern int POP_SIZE;
void* shm_ptr;
int shm_id;
int shm_sem_id;


// used by manager
void shm_create() {
  int size = (sizeof(student) * POP_SIZE);
  shm_id = shmget(SHM_KEY, size, 0600 | IPC_CREAT);
  if (shm_id == -1) {
    error("shmget\n");
  }
  shm_ptr = shmat(shm_id, NULL, 0);
  if (shm_ptr == (void*)-1) {
    error("shmat\n");
  }
  shm_sem_id = sem_create(SHM_SEM_KEY, 3);
  // writing lock is free at start
  sem_set(shm_sem_id, WRITE, 1);
}

void shm_write_students(list* students) {
  shm_write();
  node* n = students->first;
  student* arr = (student*)shm_ptr;
  for (int i = 0; i < students->length; i++) {
    student* s = (student*)n->elem;
    student* x = &arr[i];
    memcpy(x, s, sizeof(student));
    n = n->next;
  }
  shm_stop_write();
}

void shm_close_group(group* g) {
  shm_write();
  int pids[g->size];
  for (int i = 0; i < g->size;i++) {
    pids[i] = g->students[i]->pid;
  }
  student* arr = (student*)shm_ptr;
  for (int i = 0; i < POP_SIZE; i++) {
    student* x = &arr[i];
    for (int j = 0; j < g->size;j++) {
      if (x->pid == pids[j]) {
        x->invite = FALSE;
      }
    }
  }
  shm_stop_write();
}

// used by students, so read-only permission is set
void shm_get() {
  shm_id = shmget(SHM_KEY, 0, 0600);
  if (shm_id == -1) {
    error("shmget\n");
  }
  shm_ptr = shmat(shm_id, NULL, 0);
  if (shm_ptr == (void*)-1) {
    error("shmat\n");
  }
  shm_sem_id = sem_get(SHM_SEM_KEY);
}

void shm_read() {
  // waiting for writers to write
  sem_op(shm_sem_id, WRITE_WAIT, 0, TRUE);
  // increase readers by 1
  sem_op(shm_sem_id, READ, 1, TRUE);
}

void shm_stop_read() {
  // decrease readers by 1
  sem_op(shm_sem_id, READ, -1, TRUE);
}

void shm_write() {
  // increase writers waiting by 1
  sem_op(shm_sem_id, WRITE_WAIT, 1, TRUE);
  // wait for readers to go to 0
  sem_op(shm_sem_id, READ, 0, TRUE);
  // get writing lock
  sem_op(shm_sem_id, WRITE, -1, TRUE);
}

void shm_stop_write() {
  // release writing lock
  sem_op(shm_sem_id, WRITE, 1, TRUE);
  // decrease writers waiting by 1
  sem_op(shm_sem_id, WRITE_WAIT, -1, TRUE);

}

void shm_delete() {
  if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
    debug("shmctl\n");
  }
  sem_delete(shm_sem_id);
}

void shm_print() {
  shm_read();
  printf("\nSHARED MEMORY STATE\n\n");
  student* arr = (student*)shm_ptr;
  for (int i = 0; i < POP_SIZE; i++) {
    print_student(&arr[i]);
    printf("\n");
  }
  shm_stop_read();
}
