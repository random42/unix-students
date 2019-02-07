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
extern int msg_id;

// start semaphore
int sem_id;

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
  // write students to shared memory
  shm_write_students(students);
  // decrements the semaphore to let children start
  sem_op(sem_id, START_SEM, -POP_SIZE, TRUE);
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
  closed_groups = new_list();
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

student* get_student(int pid) {
  node* n = students->first;
  bool found = FALSE;
  student* s;
  while (n && !found) {
    s = n->elem;
    if (s && s->pid == pid) {
      found = TRUE;
    }
    n = n->next;
  }
  return s;
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
  msg* m = malloc(sizeof(msg));
  while (1) {
    msg_receive(msg_id, m, TRUE);
    switch(m->type) {
      case MSG_GROUP: {
        on_group(m);
        break;
      }
      case MSG_CLOSE_GROUP: {
        on_close_group(m);
        break;
      }
      default: {
        ERROR("MANAGER: Wrong message type.\n");
      }
    }
  }
}

void on_group(msg* m) {
  debug("MSG_GROUP\n");
  student* leader = get_student(m->from);
  student* new = get_student(m->data);
  group* g;
  if (leader->group) {
    g = leader->group;
    if (g->leader != leader) {
      ERROR("Sender is not the group leader. GROUP\n");
    }
    if (g->closed) {
      ERROR("Group is already closed. GROUP\n");
    }
  }
  else {
    g = new_group();
    group_add_student(g, leader);
    g->leader = leader;
    list_add(groups, g);
  }
  group_add_student(g, new);
}

void on_close_group(msg* m) {
  debug("MSG_CLOSE_GROUP\n");
  student* leader = get_student(m->from);
  int leader_num = m->data;
  group* g = leader->group;
  // if student is not in any group
  if (!g) {
    g = new_group();
    group_add_student(g, leader);
    g->leader = leader;

  }
  else if (g->leader != leader) {
    ERROR("Sender is not the group leader. CLOSE_GROUP\n");
  }
  if (g->closed) {
    ERROR("Group is already closed. CLOSE_GROUP\n");
  }
  g->closed = TRUE;
  list_add(closed_groups, g);
  shm_close_group(g);
  // next leader turn to invite
  sem_op(sem_id, TURN_SEM, leader_num + 1, TRUE);
}

void end(int signal) {
  debug("END\n");
  print_infos();
  wait_for_children();
  exit(EXIT_SUCCESS);
}

void set_votes() {

}

// initialize ipc structures
void ipc_init() {
  sem_id = sem_create(SEM_KEY, 1);
  shm_create();
  msg_init();
}

void ipc_close() {
  sem_delete(sem_id);
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
  printf("\nTempo trascorso: %lf\n", elapsed_time(&start_time));
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
