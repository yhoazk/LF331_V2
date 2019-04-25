#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static int count = 0, max_int = 5;
static struct sigaction sig;


static void signal_handler(int sig, siginfo_t* siginfo, void* ctx){
  pid_t sender_pid = siginfo->si_pid;
  if(SIGINT == sig){
    count++;
    printf("SIGINT: %d from PID: %d\n", count, sender_pid);
    return;
  } else if(SIGQUIT == sig) {
    printf("Quit signal from PID: %d\n", sender_pid);
    exit(0);
  }
  return;
}

int raise_test(){
  // print my PID
  printf("Process PID: %d started\n", (int)getpid());

  sig.sa_sigaction = *signal_handler; //?
  sig.sa_flags |= SA_SIGINFO; /* Get signal details */

  if(sigaction(SIGINT, &sig, NULL) != 0){
    printf("Error sigaction()\n");
    return errno;
  }

  if(sigaction(SIGQUIT, &sig, NULL) != 0){
    printf("Error sigaction()\n");
    return errno;
  }

  /* send signal with ctrl+c ad ctrl+ \ */
  int rcvsig;
  while(1){
    if(count < max_int){
      rcvsig = SIGINT;
    } else {
      rcvsig = SIGQUIT;
    }
    //raise(rcvsig);
    sleep(1); // sleep, note that sigint interrumpts and makes a proper wake up
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  raise_test();
  return 0;
}
