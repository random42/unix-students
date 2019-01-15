#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int sem_get(int key) {
  int id = semget(key, 0, 0);
  if (id == -1) {
    ERROR("sem_get\n");
  }
  return id;
}

int sem_create(int key, int n_sems) {
  int id = semget(key, n_sems, IPC_CREAT);
  if (id == -1) {
    ERROR("sem_create\n");
  }
  return id;
}

void sem_delete(int sem_id) {
  int r = semctl(sem_id, 0, IPC_RMID);
  if (r == -1) {
    ERROR("sem_delete\n");
  }
}

int sem_op(int sem_id, short sem_num, short op, bool wait) {
  struct sembuf s;
  s.sem_num = sem_num;
  s.sem_op = op;
  s.sem_flg = wait ? 0 : IPC_NOWAIT;
  return semop(sem_id, &s, 1);
}

void sem_set_val(int sem_id, short sem_num, short val) {
  int r = semctl(id, sem_num, SETVAL, val);
  if (r == -1) {
    ERROR("sem_set_val\n");
  }
}
