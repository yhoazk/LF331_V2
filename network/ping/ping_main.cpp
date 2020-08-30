#include "ping.h"
#include <iostream>


int main(int ac, char** av, char** ev) {
    net::types::ipv4_t ip{192,168,0,1};
    net::types::pinger png{ip};

    png.set();
    png.ping_send();
    return 0;
}