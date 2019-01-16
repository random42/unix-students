#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "conf.h"
#include "msg.h"

// message queue id
int msg_id;
int msg_size;

void msg_init() {
  msg_id = msgget(MSG_KEY, IPC_CREAT);
  msg_size = sizeof(msg) - sizeof(long);
  if (msg_id == -1) {
    ERROR("Cannot create message queue.");
  }
}

void msg_invite(int pid, int voto_AdE, int nof_elems) {
  msg m;
  m.mtype = pid;
  m.from = getpid();
  m.type = INVITE;
  m.voto_AdE = voto_AdE;
  m.nof_elems = nof_elems;
  msgsnd(msg_id, &m, msg_size, 0);
}

void msg_respond(int pid, bool response) {
  msg m;
  m.mtype = pid;
  m.from = getpid();
  m.type = RESPOND;
  m.response = response;
  msgsnd(msg_id, &m, msg_size, 0);
}

void msg_send_vote(student* s) {
  msg m;
  m.mtype = s->pid;
  m.from = getpid();
  m.type = VOTE;
  m.vote = s->vote;
  msgsnd(msg_id, &m, msg_size, 0);
}

void msg_receive(msg* buffer, bool wait) {
  int flag = wait ? 0 : IPC_NOWAIT;
  msgrcv(msg_id, buffer, msg_size, getpid(), flag);
}

void msg_group(int student) {
  msg m;
  m.mtype = getppid();
  m.from = getpid();
  m.type = GROUP;
  m.student = student;
  msgsnd(msg_id, &m, msg_size, 0);
}

void msg_close_group() {
  msg m;
  m.mtype = getppid();
  m.from = getpid();
  m.type = CLOSE_GROUP;
  msgsnd(msg_id, &m, msg_size, 0);
}

void msg_close() {
  msgctl(msg_id, IPC_RMID, NULL);
}
