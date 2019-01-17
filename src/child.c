#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "conf.h"
#include "student.h"
#include "group.h"
#include "shm.h"
#include "msg.h"
#include "sem.h"


student self;

void ipc_init();
void ipc_close();

void init() {

}

void end() {

}

int main(int argc, char* argv[]) {
  self.voto_AdE = strtoul(argv[1], NULL, 10);
  self.nof_elems = strtoul(argv[2], NULL, 10);
  print_student(&self);
}
