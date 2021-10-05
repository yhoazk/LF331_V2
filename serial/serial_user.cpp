#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "serial/serial.hpp"

int main()
{
    using namespace std::chrono_literals;
    serial_configuration cfg{};
    serial s{"/dev/ttyACM0"};
    s.config(cfg);
    std::vector<std::uint8_t> command{0, 0, 0x0A};
    int loops{10};
    while (loops-- >= 1) {
        std::cout << "loops: " << loops << std::endl;
        std::this_thread::sleep_for(500ms);
        command[0] = static_cast<std::uint8_t>(0x34);
        command[1] = static_cast<std::uint8_t>(0x34);
        s.send(command);
        std::this_thread::sleep_for(500ms);
        command[0] = static_cast<std::uint8_t>(0x36);
        command[1] = static_cast<std::uint8_t>(0x36);
        s.send(command);
    }
    return 0;
}