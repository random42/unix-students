#ifndef MANAGER_H

#define MANAGER_H

#include "random.h"
#include "conf.h"
#include "list.h"
#include "student.h"
#include "group.h"
#include "shm.h"
#include "msg.h"
#include "sem.h"


void init();
void ipc_init();
void start();
void set_signal_handler(int signal, void (*f)(int), bool atomic);
list* create_students(int num);
void spawn_student(student* s);
void wait_for_messages();
student* get_student(int pid);
void on_msg(msg* m);
void close_group(msg* m);
double elapsed_time();
void ipc_close();
void print_votes();
void print_infos();
void set_votes();
void end();



void wait_for_children();




#endif
