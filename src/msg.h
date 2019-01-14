#ifndef MSG_H

#define MSG_H
#define MSG_KEY 1234
#define MSG_LENGTH

#include "conf.h"

typedef enum {INVITE,RESPONSE,VOTE,GROUP,CLOSE_GROUP} msg_t;

typedef struct {
  // pid of receiving process
  long mtype;
  // pid of sender
  int from;
  // msg type
  msg_t type;
  // INVITE data
  uint voto_AdE;
  uint nof_elems;
  // RESPONSE
  bool response;
  // VOTE and GROUP data (vote or student number)
  uint data;
} msg;

void msg_init();
void invite(int pid, uint voto_AdE, uint nof_elems);
void respond(int pid, bool response);
void send_vote(int pid, uint vote);
void receive(msg* buffer, bool wait);
void msg_close();

#endif
