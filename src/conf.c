#include <stdlib.h>
#include <stdio.h>
#include "conf.h"

config conf;

void config_init() {
  FILE* opt = fopen("opt.conf","r");
  if (!opt) {
    ERROR("Cannot read from opt.conf file.")
  }
  fscanf(opt, "%*s %lf %*s %lf %*s %lf %*s %d %*s %d", &conf.two, &conf.three, &conf.four, &conf.nof_invites, &conf.max_reject);
  fclose(opt);
}
