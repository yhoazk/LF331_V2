#include "tx_ts.h"

namespace network {
namespace timestamp {
namespace tx_ts {

int enable_sw_txts(int socket_fd) {
//    int flags = SOF_TIMESTAMP_TX_SOFTWARE; apparently SOF_TIMESTAMP_TX_SOFTWARE does not exist
    int flags = SOF_TIMESTAMPING_TX_SOFTWARE;
    int err = setsockopt(socket_fd, SOL_SOCKET, SO_TIMESTAMPING, &flags, sizeof(flags));
    if (err < 0) {
        perror("setsockopt");
    }
    return socket_fd;
}

int enable_hw_txts(int socket_fd) {
    return socket_fd;
}
/*
int get_sw_txts(const std::unique_ptr<>& buff) {
    int level, type;
    struct cmsghdr *cm;
    struct timespec *ts{nullptr};

    for (cm = CMSG_FIRSTHDR(&msg); cm != nullptr; cm=CMSG_NXTHDR(&msg,cm)) {
        if (SOL_SOCKET == level && SO_TIMESTAMPING == type) {
            ts = static_cast<timespec*> CMSG_DATA(cm);
            std::cerr << "Timestamp: " << std::to_string(ts[2].tv_sec) // why [2]?
                      << "sec " << ts[2].tv_nsec << "ns\n";
        }
    }
}
int get_sw_txts(const std::unique_ptr<rcv_buffer>& buff) {

}
*/

}  // namespace tx_ts
}  // namespace timestamp
}  // namespace network

