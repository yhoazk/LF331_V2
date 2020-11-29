/*
 * Enable HW Tx and Rx timestamping
 * Then get timestamp from Tx UDP pkgs
 * */

#include <iostream>
#include <array>

#include "hw_config.h"
#include "tx_ts.h"
#include "rx_ts.h"

extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
}

const size_t tgt_port{8080};


int main () {
    using namespace network::timestamp::tx_ts;
    network::timestamp::hw_config hw_cfg("enp9s0");
    auto opt_config = hw_cfg.read_config();
    int udp_sock_fd{-1};
    std::array<uint8_t, 512> msg_buf;
    msg_buf.fill(5);
    msg_buf[256] = '\0';
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    udp_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = INADDR_ANY;
    // needs su
    //servaddr.sin_addr.s_addr = inet_addr("192.168.0.129");
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(tgt_port);

    //int bind_err = bind(udp_sock_fd, reinterpret_cast<const struct sockaddr*>(&servaddr), sizeof(servaddr));
    int conn_err = connect(udp_sock_fd, reinterpret_cast<const struct sockaddr*>(&servaddr),
                       sizeof(servaddr));

    if(conn_err < 0) {
        perror("connect");
        return -1;
    } else {
        std::cerr << "socket is bound to interface\n";
    }

    if (opt_config){
        std::cout << "Config" << std::to_string(opt_config.value()) << '\n';
    } else {
        std::cerr << "Unable to get config or HW ts is not supported\n";
        // normally we should return here if we are only intestested in HW
        // here it continues with SW timestamps
        // is there any difference if the timestamp is set before or after bind
        // is there any difference if the timestamp is set before or after bind??
        int ok = enable_sw_txts(udp_sock_fd);
        if(ok < 0) {
            std::cerr << "Unable to set timestamp in socket\n";
        } else {
            std::cout << "Timestamps enabled in socket\n";
        }

    }
    std::cout << "Sending udp pkt\n";
    unsigned int len = sizeof(cliaddr);
    int bytes_send = sendto(udp_sock_fd, reinterpret_cast<const char*>(msg_buf.data()),
                            msg_buf.size(), MSG_CONFIRM, 
                            reinterpret_cast<const struct sockaddr*>(NULL), sizeof(servaddr));

    if (bytes_send < 0 ) {
        perror("sendto");
    } else {
        std::cout << "Bytes send: " << std::to_string(bytes_send) << '\n';
        // get timestamp of message
        auto ts = get_sw_txts(udp_sock_fd, msg_buf);
    }
    close(udp_sock_fd);
    return 0;
}
