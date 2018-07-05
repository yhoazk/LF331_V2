#include <sys/inotify.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void printEvnt(struct inotify_event *i){
    printf("    wd =%2d; ", i->wd);
    if(i->cookie > 0){
      printf("coockie=%4d; ", i->cookie );
    }
    printf("mask: ");
    if(i->mask & IN_ACCESS) printf("IN_ACCESS ");
    if(i->mask & IN_ATTRIB) printf("IN_ATTRIB ");
    if(i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if(i->mask & IN_CLOSE_WRITE) printf("IN_CLOSE_WRITE ");
    if(i->mask & IN_CREATE) printf("IN_CREATE ");
    if(i->mask & IN_DELETE) printf("IN_DELETE ");
    if(i->mask & IN_DELETE_SELF) printf("IN_DELETE_SELF ");
    if(i->mask & IN_IGNORED) printf("IN_IGNORED ");
    if(i->mask & IN_ISDIR) printf("IN_ISDIR ");
    if(i->mask & IN_MODIFY) printf("IN_MODIFY ");
    if(i->mask & IN_MOVE_SELF) printf("IN_MOVE_SELF ");
    if(i->mask & IN_MOVED_FROM) printf("IN_MOVED_FROM ");
    if(i->mask & IN_MOVED_TO) printf("IN_MOVED_TO ");
    if(i->mask & IN_OPEN) printf("IN_OPEN ");
    if(i->mask & IN_Q_OVERFLOW) printf("IN_Q_OVERFLOW ");
    if(i->mask & IN_UNMOUNT) printf("IN_UNMOUNT ");
    printf("\n");

    if(i->len > 0){
      printf("    name %s\n", i->name);
    }
}



#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc, char* argv[]){
  int inotifyFd, wd, j;

  char buf[BUF_LEN] __attribute__((aligned(8)));
  ssize_t numReas; char *p;
  struct inotify_event *event;

  if(argc < 2){
    printf("give the file as argument\n");
    exit(9);
  }

  inotifyFd = inotify_init();

  if(inotifyFd == -1){
    exit(1);
  }

  for(j=1;j<argc;++j){
    wd = inotify_add_watch(inotifyFd, argv[j], IN_ALL_EVENTS);
    if(wd == -1){
      exit(2);
    }

    printf("Watching %s using wd %d\n", argv[j], wd);
  }

  while (1) {
    numReas = read(inotifyFd, buf, BUF_LEN);
    if(numReas == 0){
      exit(3);
    }

    if(numReas == -1){
      exit(4);
    }


    printf("read %ld bytes from inotifiy fd\n", (long) numReas);

    for (p = buf; p < buf + numReas;) {
      event = (struct inotify_event*) p;
      printEvnt(event);
      p += sizeof(struct inotify_event) + event->len;
    }
  }
  exit(0);
}
