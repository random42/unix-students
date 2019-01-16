#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "conf.h"
#include "shm.h"
#include "sem.h"
#define SHM_KEY 1234
#define SHM_SEM_KEY 1253
#define READ 0 // num of readers
#define WRITE 1 // 0 if writing, 1 otherwise
#define WRITE_WAIT 2 // number of writers waiting

int shm_id;
int sem_id;

// used by manager
void* shm_create(int size) {
  shm_id = shmget(SHM_KEY, size, 0600 | IPC_CREAT);
  if (shm_id == -1) {
    ERROR("shmget\n");
  }
  void* p = shmat(shm_id, NULL, 0);
  if (p == (void*)-1) {
    ERROR("shmat\n");
  }
  sem_id = sem_create(SHM_SEM_KEY,3);
  sem_set(sem_id, WRITE, 1);
  return p;
}

// used by students, so read-only permission is set
void* shm_get() {
  shm_id = shmget(SHM_KEY, 0, 0);
  if (shm_id == -1) {
    ERROR("shmget\n");
  }
  void* p = shmat(shm_id, NULL, SHM_RDONLY);
  if (p == (void*)-1) {
    ERROR("shmat\n");
  }
  sem_id = sem_get(SHM_SEM_KEY);
  return p;
}

void shm_read() {
  // waiting for writers to write
  sem_op(sem_id, WRITE_WAIT, 0, TRUE);
  sem_op(sem_id, WRITE, 0, TRUE);
  // increase readers by 1
  sem_op(sem_id, READ, 1, TRUE);
}

void shm_stop_read() {
  // decrease readers by 1
  sem_op(sem_id, READ, -1, TRUE);
}

void shm_write() {
  // increase writers waiting by 1
  sem_op(sem_id, WRITE_WAIT, 1, TRUE);
  // wait for readers to go to 0
  sem_op(sem_id, READ, 0, TRUE);
  // get writing lock
  sem_op(sem_id, WRITE, -1, TRUE);
}

void shm_stop_write() {
  // decrease writers waiting by 1
  sem_op(sem_id, WRITE_WAIT, -1, TRUE);
  // release writing lock
  sem_op(sem_id, WRITE, 1, TRUE);
}

void shm_delete() {
  if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
    ERROR("shmctl\n");
  }
  sem_delete(sem_id);
}
