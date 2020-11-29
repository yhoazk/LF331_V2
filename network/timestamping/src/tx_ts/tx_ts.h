#ifndef TX_TS_H
#define TX_TS_H

#include <memory>
#include <chrono>
#include <iostream>

extern "C" {
#include <linux/sockios.h>
#include <linux/net_tstamp.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <poll.h>
}

namespace network {
namespace timestamp {
namespace tx_ts {

int enable_sw_txts(int socket_fd);
int enable_hw_txts(int socket_fd);

timespec get_sw_txts(int socket_fd, std::array<uint8_t, 512>& msg);
timespec get_hw_txts(int socket_fd, std::array<uint8_t, 512>& msg);

}  // namespace tx_ts
}  // namespace timestamp
}  // namespace network

#endif  // TX_TS_H
