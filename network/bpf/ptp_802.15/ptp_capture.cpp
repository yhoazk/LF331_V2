#include <iostream>
#include "lib_ptp/ptp_cap.h"

int main(int c, char** v, char** env){
    auto s = ptp::create_socket("dsafsd", 2);
    std::cout << std::to_string(s.value_or(42)) << '\n';
    return c;
}