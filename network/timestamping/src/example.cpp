/*
 * Enable HW Tx and Rx timestamping
 * Then get timestamp from Tx UDP pkgs
 * */

#include <iostream>



#include "hw_config.h"
#include "tx_ts.h"
#include "rx_ts.h"

int main () {
    network::timestamp::hw_config hw_cfg("enp9s0");
    auto opt_config = hw_cfg.read_config();
    if (opt_config){
        std::cout << "Config" << std::to_string(opt_config.value()) << '\n';
    } else {
        std::cerr << "Unable to get config or HW ts is not supported\n";
        // normally we should return here
    }
    return 0;
}
