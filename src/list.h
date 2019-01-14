#ifndef LIST_H

#define LIST_H

typedef struct n {
  void* elem;
  struct n* next;
  struct n* prev;
} node;

typedef struct {
  node* first;
  node* last;
  int length;
} list;

list* new_list();
void list_add(list* l, void* obj);
void list_remove(list* l, void* obj);
void list_remove_node(list* l, node* n);
void delete_list(list* l);
int list_contains(list* l, void* obj);
void print_list(list* l, void (*f)(void*));
void for_each(list* l, void (*f)(void*));


#endif
