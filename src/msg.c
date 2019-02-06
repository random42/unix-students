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
int msg_id;
// response queue
int msg_id;
// messages between students and manager
int msg_id;
int msg_size;

void msg_send(int msq, msg* m, int flag) {
  int r = msgsnd(msq, &m, msg_size, flag);
  if (r == -1) {
    ERROR("msgsnd\n");
  }
}

void msg_init() {
  msg_id = msgget(MSG_INVITE_KEY, IPC_CREAT | 0600);
  msg_size = sizeof(msg) - sizeof(long);
  if (msg_id == -1) {
    ERROR("msg_init");
  }
}

void msg_invite(int pid, student* self) {
  msg m;
  m.mtype = pid;
  m.from = getpid();
  m.type = MSG_INVITE;
  msg_send(msg_id, &m, 0);
}

void msg_respond(int pid, bool response) {
  msg m;
  m.mtype = pid;
  m.from = getpid();
  m.type = MSG_RESPONSE;
  m.data = (int)response;
  msg_send(msg_id, &m, 0);
}

void msg_send_vote(student* s) {
  msg m;
  m.mtype = s->pid;
  m.from = getpid();
  m.type = MSG_VOTE;
  m.data = s->vote;
  msg_send(msg_id, &m, 0);
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
  m.data = student;
  msg_send(msg_id, &m, 0);
}

void msg_close_group(int leader_num) {
  msg m;
  m.mtype = getppid();
  m.from = getpid();
  m.type = MSG_CLOSE_GROUP;
  m.data = leader_num;
  msg_send(msg_id, &m, 0);
}

void msg_close() {
  int a = msgctl(msg_id, IPC_RMID, NULL);
  if (a == -1) {
    debug("msgctl\n");
  }
}
