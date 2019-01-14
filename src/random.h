#ifndef RANDOM_H
#define RANDOM_H
#include "conf.h"

void random_init();
uint random_uint();
uint random_uint_range(uint from, uint to);
double random_zero_to_one();
uint random_nof_elems(config* conf);

#endif
