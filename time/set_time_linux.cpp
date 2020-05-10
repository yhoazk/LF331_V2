#include <iostream>
#include <chrono>
#include <cerrno>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

extern "C" {
    #include <linux/rtc.h>
    #include <sys/ioctl.h>
    #include <sys/time.h>
    #include <time.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <stdlib.h>
}

// Set HW clock to any date
// Set sysclock to any date
// sync HW and sys clock

int do_ioctl(const struct rtc_time* new_time) {
    int fd = open("/dev/rtc0", O_RDONLY);
    if (fd == -1) {
        perror("rt0: ");
        exit(errno);
    }

    if (-1 == ioctl(fd, RTC_SET_TIME, new_time)) {
        perror("RTC_SET_TIME ioctl: ");
        close(fd);
        exit(errno);
    }

    struct rtc_time updated;
    if (-1 == ioctl(fd, RTC_RD_TIME, &updated)) {
        perror("RTC_RD_TIME ioctl: ");
        exit(errno);
    }
    close(fd);
    std::cout << "Current RTC: Year: " << updated.tm_year << " Month: " << updated.tm_mon
              << " Day: " << updated.tm_mday << " Hour: " << updated.tm_hour << '\n';

    return 0;    
}



int main(int argc, char const *argv[])
{
    using namespace std;
    using namespace chrono_literals;
    const string rtc0{"/dev/rtc0"};
    struct rtc_time new_rtc, current_rtc;
    auto now = chrono::system_clock::now();
    auto tomorrow = now + 24h;
    auto hours_since_epoch = chrono::duration_cast<chrono::hours>(now.time_since_epoch()).count();
    auto hours_at_tomorrow = chrono::duration_cast<chrono::hours>(tomorrow.time_since_epoch()).count();
    
    std::cout << "Hours since epoch: " << to_string(hours_since_epoch) << '\n';
    std::cout << "Hours at tomorrow since epoch: " << to_string(hours_at_tomorrow) << '\n';

    time_t timt = chrono::system_clock::to_time_t(tomorrow);
    std::cout << "GMT: " << std::put_time(std::gmtime(&timt), "%c, %Z") << '\n';

    auto ttm = *std::gmtime(&timt);
    // Set the HW clock
    do_ioctl(reinterpret_cast<struct rtc_time*>(&ttm));
    // Set the system time
    struct timeval tv;
    struct timezone tz{0,0};
    tv.tv_sec = chrono::duration_cast<chrono::seconds>(tomorrow.time_since_epoch()).count();
    tv.tv_usec = chrono::duration_cast<chrono::microseconds>(tomorrow.time_since_epoch()).count();
    std::cout << "Setting system to sec since epoc: " << tv.tv_sec << " micros: " << tv.tv_usec << '\n';
    if(-1 ==settimeofday(&tv, &tz)) {
        std::cerr << "Error: settimeofday\n";
        //exit(errno);
    }

    struct timespec ts_tv;
    ts_tv.tv_sec = tv.tv_sec;
    // apparently it does not work with nsec set
    ts_tv.tv_nsec = 0; 
    //chrono::duration_cast<chrono::nanoseconds>(tomorrow.time_since_epoch()).count();
    if (-1 == clock_settime(CLOCK_REALTIME, &ts_tv)) {
        std::cerr << __func__ << " Error: " << to_string(errno);
        exit(errno);
    }


    return 0;
}
