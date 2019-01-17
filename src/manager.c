#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "manager.h"
#include "random.h"
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

// path to student executable
char* student_path;

list* students;

student* spawn_student() {
  debug("spawn_student");
  debug(student_path);
  student* s = new_student();
  char* voto = malloc(3);
  char* nof_elems = malloc(2);
  sprintf(voto, "%d", s->voto_AdE);
  sprintf(nof_elems, "%d", s->nof_elems);
  int child = fork();
  switch(child) {
    case -1: { // error
      ERROR("fork\n");
    }
    case 0: { // child process
      execl(student_path, "student", voto, nof_elems, NULL);
      ERROR("execl\n");
    }
    default: { // parent process
      s->pid = child;
      // add to list of students
      list_add(students, s);
    }
  }
  return s;
}

// initialize data structures and ipc
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

void start() {
  debug("start");
  spawn_student();
  print_list(students, print_student);
}

void init() {
  debug("init");
  random_init();
  //ipc_init();
  students = new_list();
  // current directory
  char* dir = getenv("PWD");
  student_path = malloc(strlen(dir)+13);
  sprintf(student_path, "%s/bin/student", dir);
}

void end() {
  ipc_close();
  wait_for_children();
}

// wait for children to end
void wait_for_children() {
  int child;
  while ((child = wait(NULL)) != -1) continue;
}

int main(int argc, char* argv[]) {
  POP_SIZE = strtoul(argv[1], NULL, 10);
  SIM_TIME = strtoul(argv[2], NULL, 10);
  atexit(end);
  init();
  start();
}
