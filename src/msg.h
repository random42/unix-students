#ifndef MSG_H

#define MSG_H
#define MSG_INVITE_KEY 1234
#define MSG_RESPONSE_KEY 12345
#define MSG_MANAGER_KEY 543534

#include "conf.h"
#include "student.h"

typedef enum {MSG_INVITE,MSG_RESPONSE,MSG_VOTE,MSG_GROUP,MSG_CLOSE_GROUP} msg_t;

typedef struct {
  // pid of receiver
  long mtype;
  // pid of sender
  int from;
  // msg type
  msg_t type;
  // depends on msg type
  int data;
} msg;

void msg_init();
void msg_invite(int pid);
void msg_respond(int pid, bool response);
void msg_send_vote(student* s);
int msg_receive(msg* buffer, bool wait);
void msg_group(int student);
void msg_close_group(int leader_num);
void msg_close();

#endif
