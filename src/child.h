#ifndef CHILD_H

#define CHILD_H

#include "student.h"

void init();
void start();
void end(int signal);
void ipc_init();

// compara studenti per voto e pid
int compare_students(student** a, student** b);
void divide_students();
void wait_for_vote();

// leader functions
void leader_flow();
void wait_turn();
void choose_mates();
bool has_better_leader(student* s);
void invite();
void wait_for_responses();
void close_group();

// non-leader functions
void non_leader_flow();
void wait_for_invite();

#endif
