#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "conf.h"
#include "student.h"
#include "group.h"
#include "shm.h"
#include "msg.h"
#include "sem.h"

extern int NOF_INVITES;
extern int MAX_REJECT;
extern int POP_SIZE;
extern int SIM_TIME;

int start_sem_id;
student* self;


void ipc_init() {
  start_sem_id = sem_get(START_SEM_KEY);
  msg_init();
  shm_get();
}

void init() {
  config_init();
  ipc_init();
}

void end(int signal) {

}

void start() {
  // increments start semaphore
  sem_op(start_sem_id, 0, 1, TRUE);
  // wait for start semaphore to be 0
  sem_op(start_sem_id, 0, 0, TRUE);
  printf("pid: %d\n", getpid());
}

int main(int argc, char* argv[]) {
  self = new_student();
  self->pid = getpid();
  self->voto_AdE = strtoul(argv[1], NULL, 10);
  self->nof_elems = strtoul(argv[2], NULL, 10);
  init();
  start();

}
