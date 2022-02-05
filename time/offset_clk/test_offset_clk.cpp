#include "gtest/gtest.h"
#include "offset_clk.h"
#include <chrono>
#include <thread>
#include <iostream>

template <typename X, typename Y = typename X::duration>
bool operator>(const std::chrono::time_point<X,Y> a, const std::chrono::time_point<X,Y> b) {
        return (a.time_since_epoch() > b.time_since_epoch());
}

template <typename X, typename Y = typename X::duration>
bool operator<(const std::chrono::time_point<X,Y> a, const std::chrono::time_point<X,Y> b) {
    return !(a>b);
}

TEST(offset_clk, should_start_zero) {
    using namespace std::chrono_literals;
    offset_clk<std::chrono::system_clock> ofclk{};
    ASSERT_EQ(ofclk.ns_offset, 0ns);
}

// is not possible to compare time_points of different clocks
// then if an offset clock is created with system_clock it cannot
// be compared with a time_point of steady_clock
TEST(offset_clk, should_update_date) {
    using namespace std::chrono_literals;
    offset_clk<std::chrono::system_clock> ofclk{};
    ASSERT_TRUE(ofclk.find_resource("first_event"));

    ofclk.set_time(std::chrono::system_clock::now() + 45s);
    ASSERT_GT(ofclk.now(), std::chrono::system_clock::now());
}

TEST(offset_clk, should_increment_date) {
    using namespace std::chrono_literals;
    offset_clk<std::chrono::system_clock> ofclk{};
    ASSERT_TRUE(ofclk.find_resource("first_event"));

    auto now = std::chrono::system_clock::now();
    std::this_thread::sleep_for(1us);
    ofclk.set_time(now);
    ASSERT_GT(ofclk.now(), now);
}


TEST(offset_clk, should_increment_date_steady) {
    using namespace std::chrono_literals;
    offset_clk<std::chrono::steady_clock> ofclk{};
    ASSERT_TRUE(ofclk.find_resource("first_event"));

    auto now = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(1us);
    ofclk.set_time(now);
    ASSERT_GT(ofclk.now(), now);
}