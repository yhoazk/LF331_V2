#ifndef HW_CONFIG_H
#define HW_CONFIG_H
#include <string>
#include <optional>
#include <cerrno>

#include <iostream>

extern "C" {
#include <arpa/inet.h>
#include <errno.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
}


namespace network {
namespace timestamp {

struct hw_config{
    struct ifreq ifrq;
    struct hwtstamp_config hw_cfg;
    int socket_fd;
    ~hw_config();
    hw_config(const std::string& iface);
    std::optional<int> read_config();
    std::optional<int> write_config(int);
};

}  // namespace timestamp
}  // namespace network


#endif  // HW_CONFIG_H
