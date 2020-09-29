#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
   size_t count=20;

    while (--count)
    {
        usleep(100000);
        ++ts.tv_sec;
        clock_settime(CLOCK_REALTIME, &ts);
    }
}

