#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include "manager.h"
#include "debug.h"

extern int POP_SIZE;
extern int SIM_TIME;

// start semaphore
int sem_id;

// path to student executable
char* student_path;

struct itimerval timer;

struct timeval start_time;

list* students;
list* groups;

int main(int argc, char* argv[]) {
  init();
  start();
}

void start() {
  debug("MANAGER\n");
  // set start time
  gettimeofday(&start_time, NULL);
  setitimer(ITIMER_REAL, &timer, NULL);
  for_each(students, spawn_student);
  // write students to shared memory
  shm_write_students(students);
  // decrements the semaphore to let children start
  sem_op(sem_id, START_SEM, -POP_SIZE, TRUE);
  debug("START\n\n");
  wait_for_messages();
}

// initialize data structures
void init() {
  atexit(ipc_close);
  // read configuration file
  config_init();
  // initialize random library
  random_init();
  ipc_init();
  // get current directory
  char* dir = getenv("PWD");
  // create students
  students = create_students(POP_SIZE);
  groups = new_list();
  // set student executable path
  student_path = malloc(strlen(dir)+13);
  sprintf(student_path, "%s/bin/student", dir);
  // set timer value
  timer.it_value.tv_sec = SIM_TIME;
  // set timer function
  set_signal_handler(SIGALRM, end, TRUE);
  set_signal_handler(SIGINT, end, TRUE);
  set_signal_handler(SIGTERM, end, TRUE);
}

// initialize ipc structures
void ipc_init() {
  debug_create(DEBUG_KEY);
  sem_id = sem_create(SEM_KEY, 2);
  shm_create();
  msg_init();
}

void ipc_close() {
  debug_close();
  sem_delete(sem_id);
  msg_close();
  shm_delete();
}

// set function to handle signals
// if atomic is TRUE then the handler cannot be interrupted by other signals
void set_signal_handler(int signal, void (*f)(int), bool atomic) {
  struct sigaction s;
  s.sa_handler = f;
  if (atomic)
    sigfillset(&s.sa_mask);
  else
    sigemptyset(&s.sa_mask);
  int r = sigaction(signal, &s, NULL);
  if (r == -1) {
    error("sigaction\n");
  }
}

list* create_students(int num) {
  list* l = new_list();
  for (int i = 0; i < num; i++) {
    list_add(l, new_student());
  }
  return l;
}

void spawn_student(student* s) {
  char* voto = malloc(3);
  char* nof_elems = malloc(2);
  sprintf(voto, "%d", s->voto_AdE);
  sprintf(nof_elems, "%d", s->nof_elems);
  int child = fork();
  switch(child) {
    case -1: { // error
      error("fork\n");
      break;
    }
    case 0: { // child process
      int r = execl(student_path, "student", voto, nof_elems, NULL);
      if (r == -1) {
        debug("execl\n");
      }
      error("execl\n");
      break;
    }
    default: { // parent process
      s->pid = child;
    }
  }
}

void wait_for_messages() {
  msg* m = malloc(sizeof(msg));
  while (1) {
    msg_receive(m, TRUE);
    if (m->type != MSG_CLOSE_GROUP) {
      error("MANAGER: Wrong message type.\n");
    }
    close_group(m);
  }
}

// first pid is the leader
group* make_group(int* pids, int size) {
  node* n = students->first;
  student* s;
  group* g = new_group();
  int found = 0;
  while (n && found < size) {
    s = n->elem;
    for (int i = 0; i < size;i++) {
      if (pids[i] == s->pid) {
        group_add_student(g, s);
        if (found == 0) {
          g->leader = s;
        }
        found++;
      }
    }
    n = n->next;
  }
  return g;
}

void close_group(msg* m) {
  debug("MSG_CLOSE_GROUP\n");
  int leader_num = m->data;
  group* g = make_group(m->students, m->elems);
  g->closed = TRUE;
  list_add(groups, g);
  shm_close_group(g);
  if (DEBUG) {
    print_group(g);
  }
  // next leader turn to invite
  sem_op(sem_id, TURN_SEM, leader_num + 1, TRUE);
}

void end(int signal) {
  debug("END\n");
  // set votes
  for_each(students, student_set_vote);
  // send votes
  for_each(students, msg_send_vote);
  // wait for children to exit
  wait_for_children();
  // print infos and exit
  print_infos();
  exit(EXIT_SUCCESS);
}

double elapsed_time(struct timeval* from) {
  double start = from->tv_sec+((double)from->tv_usec/1e6);
  double now;
  struct timeval x;
  gettimeofday(&x, NULL);
  unsigned int seconds = x.tv_sec;
  unsigned int microseconds = x.tv_usec;
  now = seconds+((double)microseconds/1e6);
  return now-start;
}

double print_ade_table() {
  int num[13];
  for (int i = 0; i < 13; i++)
    num[i] = 0;
  int sum = 0;
  node* n = students->first;
  while (n) {
    student* s = n->elem;
    num[s->voto_AdE - 18]++;
    sum += s->voto_AdE;
    n = n->next;
  }
  double avg = (double)sum / POP_SIZE;
  printf("\nVoti di Architettura degli Elaboratori\n\n");
  printf("Voto\t| Num studenti\n");
  for (int i = 0; i < 13;i++) {
    printf("%d\t| %d\n", i + 18, num[i]);
  }
  printf("\nVoto medio: %lf\n", avg);
  return avg;
}

double print_so_table() {
  int num[13];
  for (int i = 0; i < 13; i++)
    num[i] = 0;
  int sum = 0;
  node* n = students->first;
  while (n) {
    student* s = n->elem;
    num[s->vote - 18]++;
    sum += s->vote;
    n = n->next;
  }
  double avg = (double)sum / POP_SIZE;
  printf("\nVoti di Sistemi Operativi\n\n");
  printf("Voto\t| Num studenti\n");
  for (int i = 0; i < 13;i++) {
    printf("%d\t| %d\n", i + 18, num[i]);
  }
  printf("\nVoto medio: %lf\n", avg);
  return avg;
}

void print_infos() {
  double ade = print_ade_table();
  double so = print_so_table();
  printf("\nMiglioramento medio: %lf\n", so - ade);
  printf("Tempo trascorso: %lf\n", elapsed_time(&start_time));
  printf("Studenti: %d\n", students->length);
  printf("Gruppi formati: %d\n", groups->length);
}

// wait for children to exit
void wait_for_children() {
  int child;
  while ((child = wait(NULL)) != -1) continue;
}
