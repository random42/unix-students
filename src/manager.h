#ifndef MANAGER_H

#define MANAGER_H

#include "student.h"

student* spawn_student();
void ipc_init();
void ipc_close();
void start();
void wait_for_children();
void init();


#endif
