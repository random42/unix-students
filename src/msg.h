#ifndef MSG_H

#define MSG_H
#define MSG_INVITE_KEY 1234

#include "conf.h"
#include "student.h"

typedef enum {MSG_INVITE,MSG_RESPONSE,MSG_VOTE,MSG_CLOSE_GROUP} msg_t;

typedef struct {
  // pid of receiver
  long mtype;
  // pid of sender
  int from;
  // msg type
  msg_t type;
  /*
  MSG_RESPONSE: response
  MSG_VOTE: vote
  MSG_CLOSE_GROUP: leader_num
  */
  int data;
  // MSG_CLOSE_GROUP
  int students[4];
  int elems;
} msg;

void msg_init();
void msg_invite(int pid);
void msg_respond(int pid, bool response);
void msg_send_vote(student* s);
int msg_receive(msg* buffer, bool wait);
void msg_close_group(int leader_num, int* mates, int elems);
void msg_close();

#endif
