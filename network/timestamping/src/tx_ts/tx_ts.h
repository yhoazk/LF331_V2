#ifndef TX_TS_H
#define TX_TS_H

#include <memory>
#include <chrono>

extern "C" {
#include <linux/sockios.h>
#include <linux/net_tstamp.h>
#include <unistd.h>
#include <arpa/inet.h>
}

namespace network {
namespace timestamp {
namespace tx_ts {

int enable_sw_txts(int socket_fd);
int enable_hw_txts(int socket_fd);

int get_sw_txts(int socket_fd);
int get_hw_txts(int socket_fd);

}  // namespace tx_ts
}  // namespace timestamp
}  // namespace network

#endif  // TX_TS_H
