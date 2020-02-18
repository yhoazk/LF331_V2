#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <net/if.h>

/*
https://lwn.net/Articles/208755/
https://lwn.net/Articles/137299/
To print structs with the fields ind GDB set the pretty print on
(gdb) set print pretty on
(gdb) p *struct_ptr
*/
//#include "route_helpers.h"

/* manage netlink level operations */

/**
 * Prints the information for NL messages of type RTM_GETLINK
 * */
void rtnl_print_link(struct nlmsghdr *h){
  struct ifinfomsg *iface;
  struct rtattr *attribute;
  int len;
  char str_buff[30];
  unsigned char *str;
  memset(str_buff, 0, sizeof(str_buff));
  iface = NLMSG_DATA(h);
  len = h->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));

  /* loop over all attributes for the NEWLINK message */
  for (attribute = IFLA_RTA(iface); RTA_OK(attribute, len); attribute = RTA_NEXT(attribute, len)){
      switch(attribute->rta_type){
    case IFLA_IFNAME:
      printf("Interface %d : %s\n", iface->ifi_index, (char *) RTA_DATA(attribute));
      break;
    case IFLA_TXQLEN:
      printf("IFLA_TXQLEN %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_OPERSTATE:
      printf("IFLA_OPERSTATE %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_LINKMODE:
      printf("IFLA_LINKMODE %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_MTU:
      printf("IFLA_MTU %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_MIN_MTU:
      printf("IFLA_MIN_MTU %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_MAX_MTU:
      printf("IFLA_MAX_MTU %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_GROUP:
      printf("IFLA_GROUP %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_PROMISCUITY:
      printf("IFLA_PROMISCUITY %d\n", *((int*) RTA_DATA(attribute)));
      break;
    case IFLA_ADDRESS:
      str = (unsigned char*) RTA_DATA(attribute);
      snprintf(str_buff, sizeof(str_buff), "%02x:%02x:%02x:%02x:%02x:%02x", str[0], str[1],  str[2], str[3], str[4], str[5]);
      printf("IFLA_ADDRESS %s\n", str_buff);

    default:
      break;
    }
    }
}

#define NDA_RTA(r) \
	((struct rtattr *)(((char *)(r)) + NLMSG_ALIGN(sizeof(struct ndmsg))))

void link(uint8_t* buff, int rcvd){
    
}

// receive the buffer and try to get the neigh information
void handle_neig(uint8_t* buff, int rcvd){
    // as input we get the raw buffer from recvmsg
    // The ndmsg is contained as an attribute (?)
    struct nlmsghdr* hdr;
    struct rtattr* neig_attrs[NDA_MAX+1];
    struct rtattr *attr;
    struct ndmsg* neigh;

    hdr = (struct nlmsghdr*) buff;
    // the return message from RTM_GETNEIGH contains a ndmsg struct
    // is it contained as an attr?
    // http://man7.org/linux/man-pages/man7/rtnetlink.7.html
    // https://www.earth.li/~noodles/blog/2018/09/netlink-arp-presence.html
    // https://github.com/vishvananda/netlink/blob/master/neigh_linux.go#L278

    // TODO: Iterate over the nlmsghrds received
    while(NLMSG_OK(hdr, rcvd)){
    neigh = (struct ndmsg*) NLMSG_DATA(hdr);
    int len = hdr->nlmsg_len;
    len -= NLMSG_LENGTH(sizeof(*neigh));
    attr = NDA_RTA(neigh);
    len = hdr->nlmsg_len - NLMSG_LENGTH(sizeof(*neigh));
    unsigned char* str_mac;
    char mac_buf[30];
    memset(mac_buf, 0 , sizeof(mac_buf)); 
    printf("Interface index %d\n", neigh->ndm_ifindex);
    if_indextoname(neigh->ndm_ifindex, mac_buf);
    printf("Interface name %s\n", mac_buf);
    memset(mac_buf, 0 , sizeof(mac_buf)); 
    while(RTA_OK(attr, len)){
        switch (attr->rta_type){
            case NDA_LLADDR:
                str_mac = (unsigned char*)RTA_DATA(attr);
                snprintf(mac_buf, sizeof(mac_buf), " %02x:%02x:%02x:%02x:%02x:%02x",  str_mac[0], str_mac[1], str_mac[2], str_mac[3], str_mac[4], str_mac[5]); 
                printf("MAAAAC: %s\n", mac_buf);
                memset(mac_buf, 0 , sizeof(mac_buf)); 
                break;
            case NDA_DST:
                str_mac = (unsigned char*)RTA_DATA(attr);
                snprintf(mac_buf, sizeof(mac_buf), " %02x:%02x:%02x:%02x:%02x:%02x",  str_mac[0], str_mac[1], str_mac[2], str_mac[3], str_mac[4], str_mac[5]); 
                printf("DST: %s\n", mac_buf);
                memset(mac_buf, 0 , sizeof(mac_buf)); 
                break;
            default:
                break;
            }
            printf("type: %d : try int: %d try strin\n", attr->rta_type, RTA_DATA(attr));
            attr = RTA_NEXT(attr, len);
        }
//        show_ndmsg(neigh);
        hdr = NLMSG_NEXT(hdr, rcvd);
    }
    struct nda_cacheinfo *cinfo;
    // cinfo = (struct nda_cacheinfo*) 
}

void create_nlsocket_any(int* fd, int proto){
    /* SOCK_CLOEXEC:
        Set the close-on-exec flag on the new file descriptor
        from open(2) O_CLOEXEC:
        This is useful for multithread programs to avoid file descriptor leak
        in case of execve due to race conditions in the open call
        SOCK_RAW:
        NETLINK takes sock_raw and sock_dgram as types but it makes no 
        distiction between any of those
      */
    *fd = socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, proto);
}

void create_nlsocket_route(int* fd){
    create_nlsocket_any(fd, NETLINK_ROUTE);
}

int get_route(){
    struct nlmsghdr *nmsg;
    struct rtmsg *rmsg;
    struct rtattr *rta;
    uint8_t buf[1024*8];
    struct sockaddr_nl snl; // socket type specific to NETLINK, it should be casted to sockaddr
    // iovec
    struct iovec iov;       // iovec: 
    struct msghdr msg;
    static int seq;
    int rcv_len;
    int sfd; // socket file descriptor
    int i;
    // Could generic be used for ROUTE and GETLINK?
    // sfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    create_nlsocket_route(&sfd);
    // int sfd = socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_GENERIC);
    memset(&snl, 0, sizeof(snl)); // here the nl_pid is set to zero to indicate that the msg is for the kernel
    snl.nl_family = AF_NETLINK;
    if(bind(sfd, (struct sockaddr*) &snl, sizeof(snl)) < 0){
        perror("Error bind");
    }

    memset(buf, 0 , sizeof(buf));
    nmsg = (struct nlmsghdr*)buf;
    nmsg->nlmsg_len = NLMSG_LENGTH(sizeof(*nmsg) + RTA_LENGTH(4/*IPV4_LEN*/));
    nmsg->nlmsg_flags = NLM_F_REQUEST|NLM_F_ROOT|NLM_F_ATOMIC|NLM_F_DUMP;
    nmsg->nlmsg_type = RTM_GETNEIGH; // 
    // nmsg->nlmsg_type = RTM_GETLINK;  // get interface settings
    // nmsg->nlmsg_type = RTM_GETNEIGHTBL; // to get all the table, see linux/neighbour.h not documented
    nmsg->nlmsg_seq = ++seq;

    rmsg = (struct rtmsg*) (nmsg+1);
    rmsg->rtm_family = AF_INET;
    rmsg->rtm_dst_len = 0; // if 0 is specified we indicate that we want all the entries in the table
    rmsg->rtm_src_len = 0; // if 0 is specified we indicate that we want all the entries in the table

    rta = RTM_RTA(rmsg);
    rta->rta_type = RTA_DST;
    rta->rta_len = RTA_LENGTH(4); // 4 = IPV4 addr len


    // copy to rta?
    // i = htonl(0x60060606);
    // memcpy(RTA_DATA(rta), &i, 4);

    memset(&snl, 0, sizeof(snl));
    snl.nl_family = AF_NETLINK;
    iov.iov_base = nmsg;
    iov.iov_len = nmsg->nlmsg_len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = &snl;
    msg.msg_namelen = sizeof(snl);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if(sendmsg(sfd, &msg, 0) < 0)  perror("Err\n");
    //reuse the same structure to receive the response
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);
    // Which structure is returned with the recvmsg call?
    for(;;){
        if((rcv_len = recvmsg(sfd, &msg, 0)) <= 0 ){
            perror("Error recvmsg\n");
        } else if( ((struct sockaddr_nl*)(&msg.msg_name))->nl_pid == 0){
            // verify that the message was sent by the kernel and not another app
            printf("Message from Kernel\n");
        }
        printf("Received: %d\n", rcv_len);
        printf("Received nlmsg_len %d\n", nmsg->nlmsg_len);
        if(nmsg->nlmsg_len < (int) sizeof(*nmsg) || nmsg->nlmsg_len > rcv_len || nmsg->nlmsg_seq != seq){
            perror("Error recvmsg\n");
        }
        if(nmsg->nlmsg_len == NLMSG_ERROR){
            perror("Error nlmsg_len\n");
        }
        // IFLA_ADDRESS gives the MAC address/
        handle_neig(msg.msg_iov->iov_base, rcv_len);
        struct nlmsghdr *curr_msg;
        curr_msg = (struct nlmsghdr*) msg.msg_iov->iov_base;
        struct ifinfomsg *iface;
        while(NLMSG_OK(curr_msg, rcv_len)){
            iface = NLMSG_DATA(curr_msg);
            struct rtattr* attr;
            rtnl_print_link(curr_msg);
            int attr_len = curr_msg->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));
            for(attr = IFLA_RTA(iface); RTA_OK(attr, attr_len); attr = RTA_NEXT(attr, attr_len)){
                //printf("rta_type: %d \t attr_len: %d rta_data: %s\n", attr->rta_type,attr_len, (char*)RTA_DATA(attr));
            }
            if(NLMSG_DONE == curr_msg->nlmsg_type){
                return 0;
            } else{

            }
            curr_msg = NLMSG_NEXT(curr_msg, rcv_len);
        }
    }
    //i -= NLMSG_LENGTH(sizeof(*nmsg));
    i = IFA_PAYLOAD(nmsg);
    int alen = 4; //address len
    struct in_addr *inp;
    char ipv4string[INET_ADDRSTRLEN];
    struct ndmsg neig;
    /*
    Filter NUD_REACHEABLE & NUD_PERMANENT
    */
    while (RTA_OK(rta, i)) {
        // How is the RTA_DATA formatted in each case?
        switch (rta->rta_type)
        {
        case RTA_OIF: // output interface index
            printf("RTA_OIF\t ");
            break;
        case RTA_DST: // route destination address
            printf("RTA_DST\t ");
            break;
        case RTA_SRC: // route source address
            printf("RTA_SRC\t ");
            break;
        case RTA_IIF: // Input interface index
            printf("RTA_IIF\t ");
            break;
        case RTA_GATEWAY: // gateway of the route
            printf("RTA_GATEWAY\t ");
            memcpy(&ipv4string, RTA_DATA(rta), alen);
            printf("GATEWAYY: %s\n", ipv4string);
            break;
        case RTA_CACHEINFO:
            printf("RTA_CACHEINFO\t ");
            break;
        case RTA_PRIORITY:
            printf("RTA_PRIORITY\t ");
            break;
        case RTA_METRICS:
            printf("RTA_METRICS\t ");
            break;
        case RTA_PROTOINFO:
            printf("RTA_PROTOINFO\t ");
            break;
        default:
            break;
        }
        // if(rta->rta_type == RTA_GATEWAY){
            inp = (struct in_addr *)RTA_DATA(rta);
            inet_ntop(AF_INET, inp, ipv4string, INET_ADDRSTRLEN);
            printf("rta_type: %d\n",rta->rta_type);
            printf("addr: %s\n",ipv4string);
        // }
        printf("next\n");
        rta = RTA_NEXT(rta, i);
    }
    close(sfd);
    return 0;
}

int rntl_neigh(int fd){
    struct {
        struct nlmsghdr nlh;
        struct ndmsg ndm;
        char buf[512];
    } req = {
        .nlh.nlmsg_len  = NLMSG_LENGTH(sizeof(struct ndmsg)),
        .nlh.nlmsg_type = RTM_GETNEIGH,
        .nlh.nlmsg_flags= NLM_F_DUMP | NLM_F_REQUEST,
        .nlh.nlmsg_seq  = 0,
        .ndm.ndm_family = AF_NETLINK

    };

    struct sockaddr_nl nladdr;
    struct iovec iov;
    struct msghdr msg = {
        .msg_name = &nladdr,
        .msg_namelen = sizeof(nladdr),
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };
    char* buffer;
    int len = send(fd, &req, sizeof(req), 0);

    if(len < 0){
        perror("send msg:");
    }

}

int main(int argc, char** argv) {
    get_route();
    return 0;
    // int sock;
    // create_nlsocket_route(&sock);
    // printf("Socket number %d\n", sock);
    // int no_bytes = rntl_neigh(sock);
    // printf("Got %d bytes\n", no_bytes);
    // return 0;
}
