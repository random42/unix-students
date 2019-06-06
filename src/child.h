#ifndef CHILD_H
#define CHILD_H

#include "student.h"

// utils
int min(int a, int b);

// main
void init();
void start();
void end(int signal);
void ipc_init();

// compara studenti per voto e pid
void divide_students();

void wait_for_vote();
int get_parity();

// leader functions
void leader_flow();
void wait_turn();
void choose_mates();
void invite();
void wait_for_responses();
void close_group();

// non-leader functions
void non_leader_flow();
void wait_for_invite();

#endif
