#ifndef _OFFSET_CLK_
#define _OFFSET_CLK_

#include <chrono>
#include <ctime>
#include <memory>

/*
inline namespace
*/
template <typename clk>
struct offset_clk {
    explicit offset_clk() {}

    bool find_resource (const std::string& resource_name) {
        return true;
    }
    template<typename dur = typename clk::duration>
    bool set_time (const std::chrono::time_point<clk, dur> tp) {
        // get the time when the timestamp is being set
        snap_tp = clk::now();
        ns_offset = tp - snap_tp;
        return true;
    }
    // return static type?
    // need typename before 'clk::duration' because 'clk' is a dependent scope
    template <typename _clk=clk, typename dur = typename clk::duration>
    std::chrono::time_point<_clk, dur> now (void) {
        auto tp = clk::now() + ns_offset;
        return tp;
    }
    std::chrono::nanoseconds ns_offset{};
    std::chrono::time_point<clk> snap_tp{};
};


#endif // _OFFSET_CLK_