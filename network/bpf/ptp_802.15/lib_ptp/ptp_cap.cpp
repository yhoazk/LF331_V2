
#include "ptp_cap.h"

extern "C" {
#include <arpa/inet.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netpacket/packet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
}

#include <iostream>

namespace ptp {

/* Filter to be used to extract only PTP messages
    each entry is a sock_filter struct with the next fields:
        - code: OP code
        - jt: Jump true
        - jf: Jump false
        - k:  Generic field
    The BPF VM is a machine with 2 regs and a stack of 16 elements of 32bits each
    An easy way to get the operations is with tcpdump dissasemble option in this 
    case:
    tcpdump -d ether proto 0x88f7
    (000) ldh      [12]               |
    (001) jeq      #0x88f7  jt 2 jf 3 | Note that this values are absolute, while the bpf expects offset
    (002) ret      #262144            |
    (003) ret      #0                 |
 */
#define OP_AND  (BPF_ALU | BPF_AND | BPF_K)
#define OP_JEQ  (BPF_JMP | BPF_JEQ | BPF_K)
#define OP_JUN  (BPF_JMP | BPF_JA)
#define OP_LDB  (BPF_LD  | BPF_B   | BPF_ABS)
#define OP_LDH  (BPF_LD  | BPF_H   | BPF_ABS)
#define OP_RETK (BPF_RET | BPF_K)
#define OP_RET (BPF_RET | BPF_K)

const size_t prog_size = 4;
static struct sock_filter ptp_filter[4] = {
    {OP_LDH, 0, 0, 12},
    {OP_JEQ, 0, 1, ETH_P_1588},
    {OP_RET,0, 0, 262144},
    {OP_RET,0, 0, 0},
};


static struct sock_filter bpfcode[6] = {
	{ OP_LDH, 0, 0, 12          },	// ldh [12]
	{ OP_JEQ, 0, 2, ETH_P_IP    },	// jeq #0x800, L2, L5
	{ OP_LDB, 0, 0, 23          },	// ldb [23]
	{ OP_JEQ, 0, 1, IPPROTO_TCP },	// jeq #0x6, L4, L5
	{ OP_RET, 0, 0, 0           },	// ret #0x0
	{ OP_RET, 0, 0, 0xffffffff,         },	// ret #0xffffffff
};

std::optional<int> create_socket(const char* dst_mac, size_t prio ) {
    struct sockaddr_ll linklayer_sock{};
    int socket_fd, indx;

    socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (socket_fd < 0) {
        return {};
    }
    indx = 3; // interface index

    memset(&linklayer_sock, 0, sizeof(linklayer_sock));
    linklayer_sock.sll_ifindex = (unsigned int)indx;
    linklayer_sock.sll_family = AF_PACKET;
    linklayer_sock.sll_protocol = htons(ETH_P_ALL);

    if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&linklayer_sock), sizeof(linklayer_sock))) {
        std::cerr << "Error to bind socket to interface\n";
    } else {
        // if (setsockopt(socket_fd, SOL_SOCKET, SO_BINDTOIFINDEX, "wlp5s0", strlen("wlp5s0"))) {
        //     std::cerr << __func__ << " Error setsockopt BIND to idx\n";
        // }

        // if (prio > 0 and setsockopt(socket_fd, SOL_SOCKET, SO_PRIORITY, &prio, sizeof(prio))) {
        //     std::cerr << __func__ << " Error setsockopt PRIORITY to idx\n";
        // }

        if(config_socket(socket_fd, indx)) {
            std::cerr << __func__ << " Error configuring the socket\n";
        }

    }


    return {}; // Create an empty optional or use std::nullopt;
}

bool config_socket(int sfd, size_t idx) {
    bool stat{false};
    struct packet_mreq mreq;
    //struct sock_fprog prg = {prog_size, ptp_filter};
    struct sock_fprog prg = {4, ptp_filter};
    struct sock_fprog bpf = { 6, bpfcode };
    // SO_ATTACH_FILTER vs SO_ATTACH_BPF ?
    //if(setsockopt(sfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf))) { // this works
    if(setsockopt(sfd, SOL_SOCKET, SO_ATTACH_FILTER, &prg, sizeof(prg))) {   // this does not works
        std::cerr << "Error inserting filter in socket\n";
    } else {
        stat = true;
    }
    unsigned char mac_addr[] = {0x1, 0x80, 0xC2, 0x0, 0x0, 0xE};
    // Add membership to socket 
    memset(&mreq, 0, sizeof(0));
    mreq.mr_ifindex = idx;
    mreq.mr_type = PACKET_MR_MULTICAST;
    mreq.mr_alen = 6;
    memcpy(mreq.mr_address, mac_addr, 6);
    stat = (0 == setsockopt(sfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq)));
    
    return stat;
}

size_t recv(){
    return 14;
}

}  // namespace ptp