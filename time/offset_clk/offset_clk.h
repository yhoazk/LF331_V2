#ifndef _OFFSET_CLK_
#define _OFFSET_CLK_

#include <chrono>
#include <ctime>
#include <memory>

/*
*/

struct offset_clk {
    explicit offset_clk() {}

    bool find_resource (const std::string& resource_name) {
        return true;
    }
    template<typename CHRONO>
    bool set_time (const CHRONO offset) {
        return true;
    }

    template <typename clk, typename dur>
    std::chrono::time_point<clk, dur> now (void) {
        std::chrono::time_point<clk, dur> tp;
        return tp;
    }
    std::chrono::nanoseconds ns_offset{};
};


#endif // _OFFSET_CLK_