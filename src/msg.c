#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "conf.h"
#include "msg.h"

// invite and group queue
int msg_invitation;
// response queue
int msg_response;
// messages between students and manager
int msg_manager;
int msg_size;

void msg_send(int msq, msg* m, int flag) {
  int r = msgsnd(msq, &m, msg_size, flag);
  if (r == -1) {
    ERROR("msgsnd\n");
  }
}

void msg_init() {
  msg_invitation = msgget(MSG_INVITE_KEY, IPC_CREAT | 0600);
  msg_response = msgget(MSG_RESPONSE_KEY, IPC_CREAT | 0600);
  msg_manager = msgget(MSG_MANAGER_KEY, IPC_CREAT | 0600);
  msg_size = sizeof(msg) - sizeof(long);
  if (msg_invitation == -1 || msg_response == -1 || msg_manager == -1) {
    ERROR("msg_init");
  }
}

void msg_invite(int pid, student* self) {
  msg m;
  m.mtype = pid;
  m.from = getpid();
  m.type = MSG_INVITE;
  memcpy(&m.s, self, sizeof(student));
  msg_send(msg_invitation, &m, msg_size);
}

void msg_respond(int pid, bool response) {
  msg m;
  m.mtype = pid;
  m.from = getpid();
  m.type = MSG_RESPONSE;
  m.response = response;
  msg_send(msg_response, &m, 0);
}

void msg_send_vote(student* s) {
  msg m;
  m.mtype = s->pid;
  m.from = getpid();
  m.type = MSG_VOTE;
  m.vote = s->vote;
  msg_send(msg_manager, &m, 0);
}

int msg_receive(int msq, msg* buffer, bool wait) {
  int flag = wait ? 0 : IPC_NOWAIT;
  int r = msgrcv(msq, buffer, msg_size, getpid(), flag);
  if (r == -1) {
    if (!wait && errno == ENOMSG)
      return -1;
    else {
      ERROR("msgrcv\n");
    }
  }
  return 0;
}

void msg_group(int student) {
  msg m;
  m.mtype = getppid();
  m.from = getpid();
  m.type = MSG_GROUP;
  m.student = student;
  msg_send(msg_manager, &m, 0);
}

void msg_close_group() {
  msg m;
  m.mtype = getppid();
  m.from = getpid();
  m.type = MSG_CLOSE_GROUP;
  msg_send(msg_manager, &m, 0);
}

void msg_close() {
  int a = msgctl(msg_invitation, IPC_RMID, NULL);
  int b = msgctl(msg_response, IPC_RMID, NULL);
  int c = msgctl(msg_manager, IPC_RMID, NULL);
  if (a == -1 || b == -1 || c == -1) {
    debug("msgctl\n");
  }
}
