#include <stdio.h>
#include "conf.h"

FILE* u_random;


void random_init() {
  u_random = fopen("/dev/urandom", "r");
  if (!u_random) {
    ERROR("Cannot open /dev/urandom");
  }
}

uint random_uint() {
  uint r;
  fread(&r, sizeof(uint), 1, u_random);
  return r;
}

uint random_uint_range(uint from, uint to) {
  uint r = random_uint();
  return r % (to - from + 1) + from;
}

double random_zero_to_one() {
  uint precision = 100000;
  uint r = random_uint_range(1, precision);
  return (double)r / precision;
}

uint random_nof_elems(config* conf) {
  double r = random_zero_to_one();
  double two = conf->two;
  double three = two + conf->three;
  double four = 1;
  if (r <= two)
    return 2;
  else if (r <= three)
    return 3;
  else return 4;
}
