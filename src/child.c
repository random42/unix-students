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

int start_sem_id;

// lo studente del processo
student* self;

/*
  array che contiene gli studenti con voto minore o uguale in ordine
  di miglioramento di voto
*/
student** invite_array;

// numero di studenti con voto minore o uguale
int invite_size;

// numero di studenti con voto maggiore o uguale
int receive_size;

// inviti senza risposta
int pending_invites;

// inviti totali
int invites;
// rifiuti totali
int rejections;

// TRUE se puo' invitare
bool can_invite;

// TRUE se si e' leader del gruppo
// (inizialmente tutti sono leader del proprio gruppo)
bool leader = TRUE;
int group_size = 1;


int main(int argc, char* argv[]) {
  self = new_student();
  self->pid = getpid();
  self->voto_AdE = strtoul(argv[1], NULL, 10);
  self->nof_elems = strtoul(argv[2], NULL, 10);
  init();
  start();
}

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
  invite_array = create_invite_array();
  main_loop();
}

// a ogni ciclo decide se invitare uno studente o
// aspettare un invito
void main_loop() {

}

// decide il tipo di azione
action choose_action() {

}

student** create_invite_array() {
  shm_read();
  student* arr = shm_ptr;
  invite_array = malloc(sizeof(student*) * POP_SIZE-1);
  for (int i = 0; i < POP_SIZE;i++) {
    // non guardo il processo stesso
    if (arr[i].pid != self->pid) {
      // se il voto e' minore o uguale lo aggiungo alla lista di studenti
      // invitabili
      if (arr[i].voto_AdE <= self->voto_AdE) {
        invite_array[invite_size++] = &arr[i];
      }
      // se e' maggiore o uguale aumento il numero di studenti
      // che possono invitarmi
      if (arr[i].voto_AdE >= self->voto_AdE) {
        receive_size++;
      }
    }
  }
  // ordino l'array per miglioramento generale di voto
  qsort_s(invite_array, invite_size, sizeof(student*), student_imp_comp, self);
  shm_stop_read();
}

// invita uno studente
// se non ci sono studenti da invitare o sono finiti gli inviti disponibili
// ritorna FALSE, altrimenti TRUE
bool invite_one();

// risponde negativamente a tutti gli inviti in coda
void reject_pending();

// ascolta
void wait_for_invite();

void accept_invite(student* leader);

void handle_accept();

void wait_for_vote();
