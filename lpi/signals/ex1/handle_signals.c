#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
void handler(int signum){
  printf("Handler signal: %d\n", signum);
}
void handler_usr(int signum){
  printf("USER: %d\n", signum);
}
int main(){
  printf("give the command kill -2 $!\n");
  printf("give the command kill -10 $!\n");
  signal(SIGINT, handler);
  signal(SIGUSR1, handler_usr);

  pause();
  pause();
  return 0;
}
