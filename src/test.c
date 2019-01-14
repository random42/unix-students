#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"


void print(void* a) {
  printf("%d", *(int*)a);
}

void test_list() {
  list* l = new_list();
  int a = 3;
  int b = 10;
  int c = 123;
  list_add(l, &a);
  list_add(l, &b);
  list_add(l, &c);
  assert(list_contains(l, &a) == 1);
  assert(list_contains(l, &b) == 1);
  assert(list_contains(l, &c) == 1);
  list_remove(l, &b);
  assert(list_contains(l, &b) == 0);
  print_list(l, print);
}

int main() {
  test_list();
}
