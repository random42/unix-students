#include <stdlib.h>
#include <stdio.h>
#include "conf.h"

uint SIM_TIME;
uint POP_SIZE;


int main(int argc, char* argv[]) {
  POP_SIZE = strtoul(argv[1], NULL, 10);
  SIM_TIME = strtoul(argv[2], NULL, 10);
}
