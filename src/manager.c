#include "conf.h"

uint SIM_TIME;



int main(int argc, char* argv[]) {
  SIM_TIME = strtoul(argv[1],NULL,10);
}
