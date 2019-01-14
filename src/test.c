#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "conf.h"
#include "list.h"
#include "random.h"

extern config conf;

void print(void* a) {
  printf("%d", *(int*)a);
}

void test_conf() {
  //ERROR("Test error!\n")
  config_init();
  printf("%lf, %lf, %lf, %d, %d\n", conf.two, conf.three, conf.four, conf.nof_invites, conf.max_reject);
}

void test_list() {
  list* l = new_list();
  int a = 3;
  int b = 10;
  int c = 123;
  list_add(l, &a);
  list_add(l, &b);
  list_add(l, &c);
  assert(list_contains(l, &a));
  assert(list_contains(l, &b));
  assert(list_contains(l, &c));
  list_remove(l, &b);
  assert(!list_contains(l, &b));
  //print_list(l, print);
}

void test_random() {
  random_init();
  int tests = 1000;
  uint two = 0, three = 0, four = 0;
  uint total;
  for (int i = 0; i < tests; i++) {
    uint r = random_uint();
    assert(r >= 0);
    //printf("%u, ",r);
    uint from = 123;
    uint to = 876;
    uint range = random_uint_range(from, to);
    //printf("%u, ",range);
    assert(range >= from && range <= to);
    double prob = random_zero_to_one();
    //printf("%lf\n", prob);
    assert(prob >= 0 && prob <= 1);
    uint nof_elems = random_nof_elems(&conf);
    if (nof_elems == 2) two++;
    else if (nof_elems == 3) three++;
    else four++;
  }
  total = two + three + four;
  printf("2 prob: %lf, test: %lf\n", conf.two, (double)two / total);
  printf("3 prob: %lf, test: %lf\n", conf.three, (double)three / total);
  printf("4 prob: %lf, test: %lf\n", conf.four, (double)four / total);
}

int main() {
  test_conf();
  test_list();
  test_random();
}
