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
        socket_fd = -1;
    } else {
        std::cout << "SW ts enabled\n";
    }
    return socket_fd;
}

int enable_hw_txts(int socket_fd) {
    return socket_fd;
}


timespec get_sw_txts(int socket_fd, std::array<uint8_t, 1024>& buf) {
    char control [256];
    int level{0};
    int type{0};
    int cnt{0};
    int res{0};

    struct cmsghdr *cm;
    void* data = reinterpret_cast<void*>(buf.data());
    struct iovec iov = {data, buf.size()};
    struct msghdr msg;
    struct timespec *ts{nullptr};
    struct timespec *sw{nullptr};
    //struct hw_timestamp hwts;

    bzero(&control, sizeof(control));
    bzero(&msg, sizeof(msg));
    // is addr needed?

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);

    // For Tx messages the ts is in the error queue
    struct pollfd pfd {socket_fd, POLLPRI, 0};
    res = poll(&pfd, 1, /*timeout*/ 1);
    if (res < 1) {
        perror("poll");
        return {0,0};
    } else if(!(pfd.revents & POLLPRI)) {
        perror("poll got error");
        return {0,0};
    }

    cnt = recvmsg(socket_fd, &msg, MSG_ERRQUEUE);
    if (cnt < 0 ) {
        perror("recvmsg:");
    }

    for (cm = CMSG_FIRSTHDR(&msg); cm != nullptr; cm=CMSG_NXTHDR(&msg,cm)) {
        level = cm->cmsg_level;
        type = cm->cmsg_type;

        if (SOL_SOCKET == level && SO_TIMESTAMPING == type) {
            if (cm->cmsg_len < sizeof(*ts)*3) {
                 std::cerr << "msg wrong length\n";
                 return {0,0};
            }
            ts = reinterpret_cast<struct timespec*> (CMSG_DATA(cm));
            std::cerr << "Timestamp: " << std::to_string(ts[2].tv_sec) // why [2]?
                      << "sec " << ts[2].tv_nsec << "ns\n";
        }
        if (SOL_SOCKET == level && SO_TIMESTAMPNS == type) {
            if (cm->cmsg_len < sizeof(*sw)) {
                std::cerr << "msg wrong len\n";
                return {0,0};
            }
            sw = reinterpret_cast<struct timespec*>(CMSG_DATA(cm));
            //hwts->sw = timespec_to_tmv(*sw);

        }
    }
}
/*
int get_sw_txts(const std::unique_ptr<rcv_buffer>& buff) {

}
*/

}  // namespace tx_ts
}  // namespace timestamp
}  // namespace network

