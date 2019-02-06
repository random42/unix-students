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
#include "sort.h"

extern int NOF_INVITES;
extern int MAX_REJECT;
extern int POP_SIZE;
extern int SIM_TIME;

// message queues ids
extern int msg_invitation;
extern int msg_response;
// shared memory pointer
extern void* shm_ptr;

int sem_id;

// lo studente del processo
student* self;

student** all;

student** leaders;
int leaders_size;

student** non_leaders;
int non_leaders_size;

list* mates;

// inviti totali
int invites;
// rifiuti totali
int rejections;

bool leader;
int leader_num;
int group_size = 1;
int target_group_size;


int main(int argc, char* argv[]) {
  self = new_student();
  self->pid = getpid();
  self->voto_AdE = strtoul(argv[1], NULL, 10);
  self->nof_elems = strtoul(argv[2], NULL, 10);
  init();
  start();
}

void ipc_init() {
  sem_id = sem_get(SEM_KEY);
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
  sem_op(sem_id, START_SEM, 1, TRUE);
  // wait for start semaphore to be 0
  sem_op(sem_id, START_SEM, 0, TRUE);
  printf("pid: %d\n", getpid());
  invite_array = create_invite_array();
  main_loop();
}

int compare_students(student** a, student** b, void* ctx) {
  student* x = *a;
  student* y = *b;
  if (x->voto_AdE > y->voto_AdE)
    return -1;
  else if (x->voto_AdE < y->voto_AdE)
    return 1;
  else {
    if (x->pid > y->pid)
      return 1;
    else return -1;
  }
}

void switch(void** arr, int i, int j) {
  void* temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

void divide_students() {
  all = malloc(sizeof(student*) * POP_SIZE);
  leaders = all;
  student* s = (student*)shm_ptr;
  for (int i = 0; i < POP_SIZE;i++) {
    all[i] = &s[i];
  }
  shm_read();
  qsort_s(all, POP_SIZE, sizeof(student*), compare_students, NULL);
  int group_elems = 0;
  int i = 0;
  student* l = NULL;
  while (group_elems < POP_SIZE) {
    l = all[i++];
    group_elems += l->nof_elems;
    if (self->pid == l->pid) {
      // self is a leader
      leader = TRUE;
      leader_num = i;
      target_group_size = self->nof_elems;
    }
  }
  leaders_size = i;
  shm_stop_read();
  // last leader has more elems than needed and self is last leader
  if (group_elems > POP_SIZE && leader && leader_num == i-1) {
    target_group_size = nof_elems - (group_elems - POP_SIZE);
  }
  non_leaders = &all[i];
  non_leaders_size = POP_SIZE - leaders_size;
}


void wait_for_vote();

// leader functions
void leader_flow();
void wait_turn();
void choose_mates();
bool has_better_leader(student* s);
void invite();
void wait_for_responses();
void close_group();

// non-leader functions
void non_leader_flow();
void wait_for_invite();
