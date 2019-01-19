#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include "manager.h"

extern int POP_SIZE;
extern int SIM_TIME;

// start semaphore
int start_sem_id;

// path to student executable
char* student_path;

struct itimerval timer;

struct timeval start_time;

list* students;
list* groups;
list* closed_groups;

int main(int argc, char* argv[]) {
  init();
  start();
}

void start() {
  // set start time
  gettimeofday(&start_time, NULL);
  setitimer(ITIMER_REAL, &timer, NULL);
  for_each(students, spawn_student);
  // decrements the semaphore to let children start
  sem_op(start_sem_id, 0, -1, FALSE);
  wait_for_messages();
  sleep(10);
}

// initialize data structures
void init() {
  // read configuration file
  config_init();
  // initialize random library
  random_init();
  // ipc_init();
  // get current directory
  char* dir = getenv("PWD");
  // create students
  students = create_students(POP_SIZE);
  groups = new_list();
  closed_groups = new_list();
  // set student executable path
  student_path = malloc(strlen(dir)+13);
  sprintf(student_path, "%s/bin/student", dir);
  // set timer value
  timer.it_value.tv_sec = SIM_TIME;
  // set timer function
  set_signal_handler(SIGALRM, end, TRUE);
  set_signal_handler(SIGINT, end, TRUE);
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
    ERROR("sigaction\n");
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
  debug("spawn_student");
  char* voto = malloc(3);
  char* nof_elems = malloc(2);
  sprintf(voto, "%d", s->voto_AdE);
  sprintf(nof_elems, "%d", s->nof_elems);
  int child = fork();
  switch(child) {
    case -1: { // error
      ERROR("fork\n");
      break;
    }
    case 0: { // child process
      execl(student_path, "student", voto, nof_elems, NULL);
      ERROR("execl\n");
      break;
    }
    default: { // parent process
      s->pid = child;
    }
  }
}

void wait_for_messages() {
  debug("wait_for_messages");
}

void on_msg(msg* m) {

}

void on_group(msg* m) {

}

void on_close_group(msg* m) {

}

void on_sim_time(int sig) {

}

void end(int signal) {
  print_infos();
  wait_for_children();
  ipc_close();
  exit(EXIT_SUCCESS);
}


void set_votes() {

}

// initialize ipc structures
void ipc_init() {
  start_sem_id = sem_create(START_SEM_KEY, 1);
  sem_set(start_sem_id, 0, POP_SIZE + 1);
  shm_create(POP_SIZE);
  msg_init();
}

void ipc_close() {
  sem_delete(start_sem_id);
  msg_close();
  shm_delete();
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

void print_infos() {
  printf("Tempo trascorso: %lf\n", elapsed_time(&start_time));
  printf("Studenti: %d\n", students->length);
  printf("Gruppi: %d\n", groups->length);
  printf("Gruppi chiusi: %d\n", closed_groups->length);
}

void print_votes() {

}


// wait for children to exit
void wait_for_children() {
  int child;
  while ((child = wait(NULL)) != -1) continue;
}
