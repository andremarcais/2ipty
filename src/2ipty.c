#include <unistd.h>
#include <stdlib.h>
#include <pty.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <poll.h>
#include <stropts.h>
#include <fcntl.h>


void create(char const* const cmd, char const* const inf);

int main(int argc, char* argv[]) {
  if(argc != 3) {
    dprintf(2,"Ussage error: %s <cmd> <input path>\n",argv[0]);
    return 1;  }

  create( argv[1], argv[2] );

  return 0;
}


int  make_sfd(); // make signal fd, ret fd
int  make_inf(char const* const path); // make 2nd input file, ret fd
void conf_tty(int fd); // config this proc's term
void conf_pty(int ptm, int tty); // config the psudo term
void run_pty(int ptm, int sig, pid_t pid, int inf); // main loop for this app

void create(char const* const cmd, char const* const inp) {
  struct winsize sz;
  int            ptm;
  pid_t          pid;           // pid of cmd
  int            sfd;           // signal fd
  int            inf;           // input fifo

  conf_tty(0);
  ioctl(0,TIOCGWINSZ, &sz);

  switch(pid = forkpty(&ptm,NULL,NULL,&sz)) {
  case 0: execlp(cmd,cmd, NULL);
  case -1: perror("failed to fork"); exit(2); }

  inf = make_inf(inp);
  sfd = make_sfd();
  run_pty(ptm,sfd,pid,inf);

  wait(NULL);
  close(ptm); close(sfd); close(inf);
  unlink(inp);
}

void run_pty(int ptm, int sfd, pid_t pid, int inf) {
  char buf;
  struct pollfd fds[] = { { fd: ptm, events: POLLIN },
                          { fd: 0,   events: POLLIN },
                          { fd: sfd, events: POLLIN },
                          { fd: inf, events: POLLIN }, };
  int const nfds = sizeof(fds)/sizeof(fds[0]);
  while(1) {
    conf_pty(ptm,0);
    if(poll(fds,nfds,-1) == -1) { perror("failed to poll in run_pty"); exit(6); }
    else {
      if(fds[0].revents & POLLIN) {    // print contents of pty to tty
        ssize_t n = read(ptm,&buf,1);
        if(n > 0) write(1,&buf,1); }

      if(fds[1].revents & POLLIN) {    // print input from tty to pty
        ssize_t n = read(0,&buf,1);
        if(n > 0) write(ptm,&buf,1); }

      if(fds[3].revents & POLLIN) {    // print input from fifo to pty
        ssize_t n = read(inf,&buf,1);
        if(n > 0) write(ptm,&buf,1); }

      if(fds[2].revents & POLLIN) {    // send sig from tty to child
        struct signalfd_siginfo info;
        read(sfd,&info,sizeof(info));
        uint32_t const sig = info.ssi_signo;
        switch(sig) {
        case SIGCHLD:
          return;
        case SIGCONT:
        case SIGTSTP:
        case SIGINT:
        case SIGHUP:
          if( kill(pid,sig) == -1 ) perror("failed to kill");
        }
      }
    }
  }
}

int make_sfd() {
  sigset_t set;
  sigemptyset(&set);  sigaddset(&set,SIGCONT);  sigaddset(&set,SIGTSTP);
  sigaddset(&set,SIGINT);  sigaddset(&set,SIGHUP);  sigaddset(&set,SIGCHLD);
  if(sigprocmask(SIG_SETMASK,&set,NULL)) {
    perror("failed to make signal fd");
    exit(8);
  }
  int fd = signalfd(-1,&set,0);
  if(fd == -1) {
    perror("failed to make signal fd");
    exit(7);
  } else return fd;
}

int make_inf(char const* const path) {
  for(int x = 1; 1; --x)
    if(mkfifo(path,0600) == -1) {
      perror("failed to create fifo");
      if(unlink(path) == -1)
        perror("failed to delete fifo");
      if(x == 0) exit(8);
    } else break;
  int fd = open( path, O_NONBLOCK );
  if(fd == -1) {
    perror("failed to open fifo");
    exit(9); }
  return fd;
}

void conf_pty(int ptm, int tty) {
  struct winsize sz;
  ioctl(tty,TIOCGWINSZ, &sz);
  ioctl(ptm,TIOCSWINSZ, &sz);
}

void conf_tty(int fd) {
  struct termios attr;
  if( tcgetattr(fd,&attr) ) { perror("conf_tty failed"); exit(3); }
  attr.c_lflag &= ~ICANON & ~ECHO;
  if( tcsetattr(fd, TCSANOW, &attr) ) { perror("conf_tty failed"); exit(4); }
}
