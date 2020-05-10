#include <iostream>
#include <chrono>
#include <cerrno>
#include <fstream>
#include <string>

extern "C" {
    #include <linux/rtc.h>
    #include <sys/ioctl.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <stdlib.h>
}

// Set HW clock to any date
// Set sysclock to any date
// sync HW and sys clock

int main(int argc, char const *argv[])
{
    using namespace std;
    using namespace chrono_literals;
    constexpr char* rtc0 = "/dev/rtc0";
    struct rtc_time new_rtc, current_rtc;
    auto now = chrono::system_clock::now();
    auto tomorrow = now + 24h;
    auto hours_since_epoch = chrono::duration_cast<chono::hours>(now.time_since_epoch().count());
    std::cout << "Hours since epoch: " << to_string(hours_since_epoch) << 




    return 0;
}
