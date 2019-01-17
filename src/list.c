#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "list.h"


list* new_list() {
  list* l = malloc(sizeof(list));
  l->first = NULL;
  l->last = NULL;
  l->length = 0;
  return l;
}

void list_add(list* l, void* obj) {
  node* n = malloc(sizeof(node));
  n->elem = obj;
  n->next = NULL;
  n->prev = l->last;
  if (l->length == 0) {
    l->first = n;
  } else {
    l->last->next = n;
  }
  l->length++;
  l->last = n;
}

void list_remove(list* l, void* obj) {
  node* n = l->first;
  while (n != NULL) {
    if (n->elem == obj)
      list_remove_node(l, n);
    n = n->next;
  }
}

void list_remove_node(list* l, node* n) {
  if (l->first == n)
    l->first = n->next;
  else
    n->prev->next = n->next;
  if (l->last == n)
    l->last = n->prev;
  else
    n->next->prev = n->prev;
  l->length--;
  free(n);
}

void delete_list(list* l) {
  node* n = l->first;
  node* next;
  while (n != NULL) {
    next = n->next;
    free(n);
    n = next;
  }
  free(l);
}

int list_contains(list* l, void* obj) {
  node* n = l->first;
  int found = 0;
  while (n != NULL && !found) {
    found = n->elem == obj;
    n = n->next;
  }
  return found;
}

void print_list(list* l, void (*f)(void*)) {
  if (l == NULL) {
    printf("NULL");
    return;
  }
  char* sep = ", ";
  printf("[");
  node* n = l->first;
  while (n != NULL) {
    f(n->elem);
    if (n != l->last)
      printf("%s", sep);
    n = n->next;
  }
  printf("]\n");
}

void for_each(list* l, void (*f)(void*)) {
  node* n = l->first;
  while (n != NULL) {
    f(n->elem);
    n = n->next;
  }
}
