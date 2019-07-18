#include <unistd.h>
#include <pty.h>
#include <stdio.h>


void create();
void play_pause();
void forward();
void backward();
void forward_a_lot();
void backward_a_lot();
void skip();

int main(int argc, char* argv[]) {
  if(argc != 2) {
    perror("Ussage error.");
    return 1;  }

  if(argv[1] == "create")          create();
  if(argv[1] == "play_pause")      play_pause();
  if(argv[1] == "forward")         forward();
  if(argv[1] == "backward")        backward();
  if(argv[1] == "forward_a_lot")   forward_a_lot();
  if(argv[1] == "backward_a_lot")  backward_a_lot();
  if(argv[1] == "skip")            skip();

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
