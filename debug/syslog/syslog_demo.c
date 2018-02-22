#include <sys/syslog.h>

int main(int argc, char const *argv[]) {
  setlogmask(LOG_UPTO (LOG_NOTICE));

  openlog("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

  syslog(LOG_NOTICE, "Program started by user %d", getuid());
  syslog(LOG_INFO, "A tree falls in a forest");

  closelog();

  return 0;
}
