
#ifndef PING_H__
#define PING_H__


#include <iostream>
#include <array>
#include <optional>


extern "C" {
    #include <netinet/ip_icmp.h>
    #include <sys/socket.h>
    #include <sys/file.h>
    #include <netdb.h>
    #include <arpa/inet.h>
}

namespace net {
namespace types {
using ipv4_t = std::array<uint8_t, 4>;
using mac_t = std::array<uint8_t, 6>;
constexpr size_t ipv4_len = std::tuple_size<ipv4_t>::value;
constexpr size_t man_len = std::tuple_size<mac_t>::value;

/* Find the best order for the members of this struct */
struct ping_msg {
    uint32_t control_message;
    mac_t mac;
    ipv4_t ip;
    uint8_t icmp_code;
};

bool send_ping();
// std::optional<ping_msg> ping_rcv();


uint16_t checksum(uint16_t* addr, size_t len) {
    int nleft = len;
    uint16_t* w = addr;
    int32_t sum;
    uint16_t csum;

    while(nleft > 0) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == -1) {
        *(uint8_t* ) (&csum) = *(uint8_t*) w;
    }
    return csum;
}


class pinger{
ipv4_t dst_ip;
struct sockaddr dst;
std::array<uint8_t, 4096> icmp_buff;
struct icmp* icmp_msg;
struct sockaddr_in* to{reinterpret_cast<struct sockaddr_in*>(&dst)};
struct sockaddr_in pktaddr;
struct protoent* prot;
size_t nping;
size_t datalen{64-8};
int sfd{0};

public:

explicit pinger(const ipv4_t& ip) : dst_ip{ip},
        icmp_msg{reinterpret_cast<struct icmp*>(icmp_buff.data())}, nping{0} {
            icmp_msg->icmp_code = 0;
            icmp_msg->icmp_type = ICMP_ECHO;
            icmp_msg->icmp_cksum = 0;
            icmp_msg->icmp_seq = nping;
            icmp_msg->icmp_id = 0xFEFA; // unique ID?

        }

bool set(){
    to->sin_family = AF_INET;
    to->sin_addr.s_addr = inet_addr("192.168.0.1"); // string x.x.x.x to net
    if ((prot = getprotobyname("icmp")) == NULL) {
        std::cerr << "getprotobyname failed\n";
        exit(errno);
    }

    if ( (sfd = socket(AF_INET, SOCK_RAW, prot->p_proto)) < 0) {
        std::cerr << "error opening socket\n";
        exit(errno);
    }
    return true;
}

bool ping_send() {
    auto xd = sendto(sfd, &icmp_buff[0], 64-8, 0, reinterpret_cast<struct sockaddr*>(to), sizeof(to));
    std::cerr << "MSG sent: " << std::to_string(xd) << '\n';
    nping++;
    return xd > 0;
}

};



}  // namespace types
}  // namespace net 
#endif  // PING_H__