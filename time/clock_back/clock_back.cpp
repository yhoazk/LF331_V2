#include <vector>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <chrono>
#include <cerrno>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <thread>

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

// Sample the current clock
// Set clock to go backwards
// store the samples in a csv file 


/** 
 * Get samples of the current clock
 * */
void sampler (void) {
    using namespace std::chrono_literals;
    constexpr size_t sample_count{1000};
    std::vector<std::chrono::nanoseconds> samples{};
    samples.reserve(sample_count);
    std::cout << "Sample size: " << samples.size() << std::endl;

    for (size_t i{0}; i < sample_count; ++i) {
        samples.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()));
        std::this_thread::sleep_for(1ms);
    }

    std::stringstream ss;
    for (const auto s : samples) {
        ss << std::to_string(s.count()) << ",\n";
    }
    std::ofstream f{"/tmp/samples.log"};
    f << ss.str();

}

/** 
 * Chages the date of the system clock in different
 * magnitudes to check if it goes back in steps or
 * an application sees it going back
 * */
void changer () {
    using namespace std::chrono_literals;
    auto now = std::chrono::system_clock::now();
    std::cout << "Charger now date: " << now.time_since_epoch().count() << std::endl;
    auto n_secs_before = now - 50s;
    time_t timt = std::chrono::system_clock::to_time_t(n_secs_before);
    // Set the system time
    struct timeval tv;
    struct timezone tz{0,0};
    if(-1 == gettimeofday(&tv, &tz)) {
        std::cerr << "Error: settimeofday:" << std::strerror(errno) << std::endl;
    }

    std::cout << "Original sec since epoc: " << tv.tv_sec << " micros: " << tv.tv_usec << '\n';
    tv.tv_sec = tv.tv_sec - 10;
    std::cout << "Setting system to sec since epoc: " << tv.tv_sec << " micros: " << tv.tv_usec << '\n';


    if(-1 == settimeofday(&tv, &tz)) {
        std::cerr << "Error: settimeofday:" << std::strerror(errno) << std::endl;
    }

}

void setter() {
    time_t _time;
    time(&_time);
    // substract the offset before parsing
    _time = _time - 5;
    auto converted_time = *gmtime(&_time);
    std::string dat{"timedatectl set-time "};
    dat += std::to_string(converted_time.tm_hour);
    dat += ":";
    dat += std::to_string(converted_time.tm_min);
    dat += ":";
    dat += std::to_string(converted_time.tm_sec);

    std::cout << "Date to be set: " << dat << std::endl;
    system(dat.c_str());
}


int main()
{
    using namespace std;
    using namespace chrono_literals;

    std::thread s(sampler);
    std::this_thread::sleep_for(55ms);
    std::thread c(setter);

    s.join();
    c.join();
    return 0;
}
