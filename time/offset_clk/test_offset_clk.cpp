#include "gtest/gtest.h"
#include "offset_clk.h"
#include <chrono>

TEST(offset_clk, should_start_zero) {
    using namespace std::chrono_literals;
    offset_clk ofclk{};
    ASSERT_EQ(ofclk.ns_offset, 0ns);
}