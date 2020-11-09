#include <time.h>
#include <unistd.h>

int main() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    while (1) {
        usleep(100000);
        ++ts.tv_sec;
        clock_settime(CLOCK_REALTIME, &ts);
    }
    
}
