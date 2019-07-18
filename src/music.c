#include <unistd.h>
#include <stdlib.h>
#include <pty.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/wait.h>


void create();
void play_pause();
void forward();
void backward();
void forward_a_lot();
void backward_a_lot();
void skip();

int main(int argc, char* argv[]) {
  if(argc != 2) {
    dprintf(2,"Ussage error.");
    return 1;  }

#define IF_ARGV(FN) if(!strcmp( argv[1], #FN )) FN()
  IF_ARGV(create);
  IF_ARGV(play_pause);
  IF_ARGV(forward);
  IF_ARGV(backward);
  IF_ARGV(forward_a_lot);
  IF_ARGV(backward_a_lot);
  IF_ARGV(skip);

  return 0;
}


void create();// { ... }
void play_pause() {}
void forward() {}
void backward() {}
void forward_a_lot() {}
void backward_a_lot() {}
void skip() {}


void conf_tty();
void run_pty(int ptm);

void create() {
  char const* const cmd = "mpsyt";
  int *ptm;
  conf_tty(0);
  switch(forkpty(ptm,NULL,NULL,NULL)) {
  case 0: execlp(cmd,cmd);
  case -1: perror("failed to fork"); exit(2);
  default: run_pty(*ptm); wait(NULL);
  }
}

void run_pty(int ptm) {
  while(1) {
    
  }
}

void conf_tty(int fd) {
  struct termios* attr = malloc(sizeof(struct termios));
  if( tcgetattr(fd,attr) ) { perror("conf_tty failed"); exit(3); }
  attr->c_lflag &= ~ICANON & ~ECHO;
  if( tcsetattr(fd, TCSANOW, attr) ) { perror("conf_tty failed"); exit(4); }
}
