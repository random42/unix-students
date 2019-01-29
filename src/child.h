#ifndef CHILD_H

#define CHILD_H

typedef enum {INVITE, RESPOND, } action;

void init();
void start();
void end(int signal);
void ipc_init();

// a ogni ciclo decide se invitare uno studente o
// aspettare un invito
void main_loop();

// decide il tipo di azione
action choose_action();

student** create_invite_array();

// invita uno studente
// se non ci sono studenti da invitare o sono finiti gli inviti disponibili
// ritorna FALSE, altrimenti TRUE
bool invite_one();

// risponde negativamente a tutti gli inviti in coda
void reject_pending();

// ascolta
void wait_for_invite();

void accept_invite(student* leader);

void handle_accept();

void wait_for_vote();


#endif
