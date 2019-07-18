#include <unistd.h>
#include <pty.h>
#include <stdio.h>
#include <string.h>


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


void create() {
  char const* const cmd = "mpsyt";
}

void play_pause() {}
void forward() {}
void backward() {}
void forward_a_lot() {}
void backward_a_lot() {}
void skip() {}
