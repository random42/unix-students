#ifndef CHILD_H

#define CHILD_H

#include "student.h"

// utils
int min(int a, int b);
int improvement(student* leader, student* s);
int compare_vote(student** a, student** b, void* ctx);
int compare_improvement(student** a, student** b, student** leader);

// main
void init();
void start();
void end(int signal);
void ipc_init();

// compara studenti per voto e pid
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
