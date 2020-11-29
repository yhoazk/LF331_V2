#include "hw_config.h"

namespace network {
namespace timestamp {

hw_config::hw_config(const std::string& iface): ifrq{}, hw_cfg{}, socket_fd{-1} {
    std::cout << "hw_config started\n";
    memset(&ifrq, 0, sizeof(ifrq));
    memset(&hw_cfg, 0, sizeof(hw_cfg));
    socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == socket_fd) {
        //std::runtime_error(std::strerror(errno).str().c_str());
        std::cerr << "Unable to oopen socket\n";
    }
    ifrq.ifr_data = reinterpret_cast<char*>(&hw_cfg);

    strncpy(ifrq.ifr_name, iface.c_str(), sizeof(ifrq.ifr_name) - 1);
}

hw_config::~hw_config() {
    if(socket_fd != -1) {
        std::cout << "closing socket\n";
        close(socket_fd);
    }
}

std::optional<int> hw_config::read_config() {
    int err = ioctl(socket_fd, SIOCGHWTSTAMP, &ifrq);
    if(err < 0 ) {
        perror("ioctl");
        return std::nullopt;
    }
    return std::optional<int>{hw_cfg.tx_type};

}

std::optional<int> hw_config::write_config(int cfg) {
    return std::optional<int>{8};
}

}  // namespace timestamp
}  // namespace network

