#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>

int get_route(){
    struct nlmsghdr *nmsg;
    struct rtmsg *rmsg;
    struct rtattr *rta;
    uint8_t buf[512];
    struct sockaddr_nl snl;
    struct iovec iov;
    struct msghdr msg;
    static int seq;
    int rcv_len;

    int i;

    int sfd = socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE);
    memset(&snl, 0, sizeof(snl)); // here the nl_pid is set to zero to indicate that the msg is for the kernel
    snl.nl_family = AF_NETLINK;
    if(bind(sfd, (struct sockaddr*) &snl, sizeof(snl)) < 0){
        printf("Error bind \n");
    }

    memset(buf, 0 , sizeof(buf));
    nmsg = (struct nlmsghdr*)buf;
    nmsg->nlmsg_len = NLMSG_LENGTH(sizeof(*nmsg) + RTA_LENGTH(4/*IPV4_LEN*/));
    nmsg->nlmsg_flags = NLM_F_REQUEST|NLM_F_ROOT|NLM_F_ATOMIC;
    nmsg->nlmsg_type = RTM_GETNEIGH;
    nmsg->nlmsg_seq = ++seq;

    rmsg = (struct rtmsg*) (nmsg+1);
    rmsg->rtm_family = AF_INET;
    rmsg->rtm_dst_len = 0; // if 0 is specified we indicate that we want all the entries in the table
    rmsg->rtm_src_len = 0; // if 0 is specified we indicate that we want all the entries in the table
    
    rta = RTM_RTA(rmsg);
    rta->rta_type = RTA_DST;
    rta->rta_len = RTA_LENGTH(4); // 4 = IPV4 addr len


    // copy to rta?
    i = htonl(0x60060606);
    memcpy(RTA_DATA(rta), &i, 4);

    memset(&snl, 0, sizeof(snl));
    snl.nl_family = AF_NETLINK;
    iov.iov_base = nmsg;
    iov.iov_len = nmsg->nlmsg_len;

    memset(&msg, 0, sizeof(msg));
    msg.msg_name = &snl;
    msg.msg_namelen = sizeof(snl);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if(sendmsg(sfd, &msg, 0) < 0){
        printf("Err\n");
    }

    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);

    if((rcv_len = recvmsg(sfd, &msg, 0)) <= 0 ){
        perror("Error recvmsg\n");
    }

    if(nmsg->nlmsg_len < (int) sizeof(*nmsg) || nmsg->nlmsg_len > rcv_len || nmsg->nlmsg_seq != seq){
        perror("Error recvmsg\n");
    }
    if(nmsg->nlmsg_len == NLMSG_ERROR){
        perror("Error nlmsg_len\n");
    }

    struct nlmsghdr *curr_msg;
    curr_msg = (struct nlmsghdr*) msg;
    struct ifinfomsg *iface;
    while(NLMSG_OK(curr_msg, rcv_len)){

        if(NLMSG_DONE == curr_msg->nlmsg_type){

        } else{

        }
        curr_msg = NLMSG_NEXT(msg, rcv_len);
    }

    //i -= NLMSG_LENGTH(sizeof(*nmsg));
    i = IFA_PAYLOAD(nmsg);
    int alen = 4; //address len
    struct in_addr *inp;
    char ipv4string[INET_ADDRSTRLEN];
    /*
    Filter NUD_REACHEABLE & NUD_PERMANENT
    */
    while (RTA_OK(rta, i)) {
        if (rta->rta_type == RTA_GATEWAY) {
            memcpy(&ipv4string, RTA_DATA(rta), alen);
            printf("GATEWAYY\n");
        } 
        // if(rta->rta_type == RTA_GATEWAY){
            inp = (struct in_addr *)RTA_DATA(rta);
            inet_ntop(AF_INET, inp, ipv4string, INET_ADDRSTRLEN);
            printf("addr: %s",ipv4string);
        // }
        printf("next\n");
        rta = RTA_NEXT(rta, i);
    }
    return 0;
}

int main(int argc, char** argv) {
    get_route();
    return 0;
}