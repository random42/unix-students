#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "conf.h"
#include "student.h"
#include "group.h"
#include "shm.h"
#include "msg.h"
#include "sem.h"

int SIM_TIME;
int POP_SIZE;

// start semaphore
int start_sem_id;

list* students;


// initialize data structures and ipc
void init() {
  start_sem_id = sem_create(START_SEM_KEY, 1);
  shm_create(POP_SIZE);
  msg_init();
}

void end() {
  sem_delete(start_sem_id);
  msg_close();
  shm_delete();
}

void start() {

}

int main(int argc, char* argv[]) {
  POP_SIZE = strtoul(argv[1], NULL, 10);
  SIM_TIME = strtoul(argv[2], NULL, 10);
  init();
}
