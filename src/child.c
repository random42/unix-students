#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "random.h"
#include "conf.h"
#include "student.h"
#include "group.h"
#include "shm.h"
#include "msg.h"
#include "sem.h"
#include "sort.h"
#include "child.h"
#include "debug.h"

extern int NOF_INVITES;
extern int MAX_REJECT;
extern int POP_SIZE;
extern int SIM_TIME;

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

// is self leader?
bool leader;
// mates' pid
int mates[3];
int leader_num;
int target_group_size;

// utility functions

int min(int a, int b) {
  return a < b ? a : b;
}

// main

int main(int argc, char* argv[]) {
  init();
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
  debug_get(DEBUG_KEY);
  random_init();
  config_init();
  ipc_init();
}

void start() {
  leader = FALSE;
  // increments start semaphore
  sem_op(sem_id, START_SEM, 1, TRUE);
  // wait for start semaphore to be 0
  sem_op(sem_id, START_SEM, 0, TRUE);
  divide_students();
  if (leader) {
    leader_flow();
  }
  else {
    non_leader_flow();
  }
  wait_for_vote();
}

// divide between leaders and non-leaders
void divide_students() {
  shm_read();
  all = malloc(sizeof(student*) * POP_SIZE);
  leaders = all;
  student* s = (student*)shm_ptr;
  // copy addresses into array
  for (int i = 0; i < POP_SIZE;i++) {
    all[i] = &s[i];
  }
  // sort students by vote
  qsort_s(all, POP_SIZE, sizeof(student*), student_vote_comp, NULL);
  ONCE {
    P;
    printf("\nALL\n");
    student_print_array(all, POP_SIZE);
    printf("\n");
    V;
  }
  int total = 0;
  int i = 0;
  student* l = NULL;
  // choose leaders among best votes until they fill POP_SIZE
  while (total < POP_SIZE) {
    l = all[i];
    int elems = min(l->nof_elems, NOF_INVITES + 1);
    total += elems;
    if (self->pid == l->pid) {
      // self is a leader
      leader = TRUE;
      leader_num = i;
      target_group_size = elems;
    }
    i++;
  }
  leaders_size = i;
  ONCE {
    P;
    printf("\nLEADERS\n");
    student_print_array(leaders, leaders_size);
    printf("\n");
    V;
  }
  shm_stop_read();
  // last leader has more elems than needed and self is last leader
  if (leader && leader_num == leaders_size-1 && total > POP_SIZE) {
    target_group_size -= (total - POP_SIZE);
    debug("Last group size %d\n", target_group_size);
  }
  non_leaders = &all[i];
  non_leaders_size = POP_SIZE - leaders_size;
  ONCE {
    P;
    printf("\nNON LEADERS\n");
    student_print_array(non_leaders, non_leaders_size);
    printf("\n");
    V;
  }
}

void wait_for_vote() {
  msg m;
  msg_receive(&m, TRUE);
  if (m.type != MSG_VOTE) {
    error("Not MSG_VOTE\n");
  }
  self->vote = m.data;
  P;
  printf("%d I got from %d to %d!\n", self->pid, self->voto_AdE, self->vote);
  V;
  exit(EXIT_SUCCESS);
}

void leader_flow() {
  wait_turn();
  debug("LEADER: %d, pid: %d, vote: %d, nof_elems: %d, target: %d\n", leader_num, self->pid, self->voto_AdE, self->nof_elems, target_group_size);
  choose_mates();
  invite();
  wait_for_responses();
  close_group();
}

void wait_turn() {
  // aspetta che il semaforo abbia il suo numero di leader e lo manda a 0
  // dopo aver chiuso il gruppo il manager aumentera' il semaforo di leader_num + 1
  // cosi che il prossimo leader faccia il suo gruppo
  sem_op(sem_id, TURN_SEM, -leader_num, TRUE);
}

void choose_mates() {
  shm_read();
  // sort students by their improvement if in self group
  qsort_s(non_leaders, non_leaders_size, sizeof(student*), student_imp_comp, self);
  if (DEBUG) {
    P;
    printf("\nNON LEADERS SORTED\n");
    student_print_array(non_leaders, non_leaders_size);
    V;
  }
  // find mates
  int m = 0;
  int i = 0;
  while (m < target_group_size - 1 && i < non_leaders_size) {
    student* s = non_leaders[i++];
    if (s->invite) {
      mates[m++] = s->pid;
    }
  }
  if (DEBUG) {
    P;
    printf("MATES:[");
    for (int i = 0; i < m;i++) {
      printf("%d ",mates[i]);
    }
    printf("]\n");
    V;
  }
  if (m < target_group_size - 1) {
    debug("Not enough mates: %d, %d\n", m, target_group_size-1);
  }
  shm_stop_read();
}

void invite() {
  debug("INVITE\n");
  for (int i = 0; i < target_group_size - 1;i++) {
    msg_invite(mates[i]);
  }
}

void wait_for_responses() {
  debug("WAITING FOR %d RESPONSES\n", target_group_size -1);
  msg m;
  for (int i = 0; i < target_group_size - 1;i++) {
    msg_receive(&m, TRUE);
    debug("RESPONSE: %d\n", i);
    if (m.type != MSG_RESPONSE) {
      debug("Message is not of type MSG_RESPONSE\n");
    }
  }
}

void close_group() {
  debug("CLOSING GROUP\n");
  msg_close_group(leader_num, mates, target_group_size);
}

void non_leader_flow() {
  wait_for_invite();
}

void wait_for_invite() {
  msg m;
  debug("WAITING FOR INVITE\n");
  msg_receive(&m, TRUE);
  if (m.type != MSG_INVITE) {
    debug("Expected invite\n");
  }
  debug("RECEIVE\n");
  msg_respond(m.from, TRUE);
}
