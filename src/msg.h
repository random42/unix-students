#ifndef MSG_H

#define MSG_H
#define MSG_KEY 1234
#define MSG_LENGTH

#include "conf.h"
#include "student.h"

typedef enum {INVITE,RESPONSE,VOTE,GROUP,CLOSE_GROUP} msg_t;

typedef struct {
  // pid of receiving process
  long mtype;
  // pid of sender
  int from;
  // msg type
  msg_t type;
  // INVITE data
  int voto_AdE;
  int nof_elems;
  // RESPONSE
  bool response;
  // VOTE
  int vote;
  // GROUP
  int student;
} msg;

void msg_init();
void msg_invite(int pid, int voto_AdE, int nof_elems);
void msg_respond(int pid, bool response);
void msg_send_vote(student* s);
void msg_receive(msg* buffer, bool wait);
void msg_group(int student);
void msg_close_group();
void msg_close();

#endif
