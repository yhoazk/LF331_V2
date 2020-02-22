#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <linux/netlink.h>
#include <arpa/inet.h>
#ifdef __sunos__
#include <sys/sockio.h>
#endif

#define inaddrr(x) (*(struct in_addr *) &ifr->x[sizeof sa.sin_port])

#define IFRSIZE ((int)(size * sizeof (struct ifreq)))

int main (){
    unsigned char *u = NULL;
    int sockfd, sockfd_arp;
    int size = 1;
    struct ifreq *ifr;
    struct ifconf ifc;
    struct sockaddr_in sa;
    struct arpreq arp;
    char macStr[128];
    memset(&arp, 0, sizeof(struct arpreq));
    
    if (0 > (sockfd_arp = socket(AF_INET, SOCK_DGRAM, NETLINK_ROUTE))){ printf("no socket for arp\n");}

    // if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP))){
    if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, 0))){
        printf ("Error: Unable to open socket.\n");
        exit (1);
    }

    ifc.ifc_len = IFRSIZE;
    ifc.ifc_req = NULL;

    do {
        ++size;
        /* realloc buffer size until no overflow occurs */
        printf("Interface\n");
        if (NULL == (ifc.ifc_req = (struct ifreq*)realloc(ifc.ifc_req, IFRSIZE))) {
            printf ("Error: Unable to allocate mememory.\n");
            exit (1);
        }

        ifc.ifc_len = IFRSIZE;
        if (ioctl(sockfd, SIOCGIFCONF, &ifc)) {
            printf ("Error: ioctl SIOCFIFCONF.\n");
            exit (1);
        }
    } while (IFRSIZE <= ifc.ifc_len);

    ifr = ifc.ifc_req;

    while ((char *) ifr < (char *) ifc.ifc_req + ifc.ifc_len) {
        printf("Interface: %s\n", ifr->ifr_name);
        printf("IP Address: %s\n", inet_ntoa(inaddrr(ifr_addr.sa_data)));


        u = NULL;
#if defined(__linux__)
        //if (0 == ioctl(sockfd, SIOCGIFHWADDR, ifr))
        ioctl(sockfd, SIOCGIFHWADDR, ifr);
        // ioctl(sockfd, SIOCGIFCONF, ifr);
            u = (unsigned char *) &ifr->ifr_addr.sa_data;
       // else
       // {
#endif
            arp.arp_pa.sa_family = AF_INET;                  ;// ifr->ifr_addr;
            memcpy(arp.arp_ha.sa_data, ifr->ifr_addr.sa_data, 13);
            memcpy(arp.arp_pa.sa_data, u, 13);
            arp.arp_flags |= ATF_COM;
            arp.arp_ha = ifr->ifr_addr;
            strcpy(&arp.arp_dev, ifr->ifr_name); // device name is needed
            if (0 == ioctl (sockfd_arp, SIOCGARP, &arp)) //https://github.com/torvalds/linux/blob/master/net/ipv4/arp.c#L1173 neigh_lookup is defined in: https://github.com/torvalds/linux/blob/master/net/core/neighbour.c#L521
                u = (unsigned char *) arp.arp_ha.sa_data;
            else
                perror ("Error during ioctl");

/********************************************************************/
            memcpy(arp.arp_ha.sa_data, u, 13);
            arp.arp_flags |= ATF_COM;
            // arp.arp_ha = ifr->ifr_addr.sa_data;
            // strcpy(&arp.arp_dev, ifr->ifr_name);
            if (0 == ioctl (sockfd_arp, SIOCGARP, &arp))
                u = (unsigned char *) arp.arp_ha.sa_data;
            else
                perror ("Error during ioctl");
/*********************************************************************/

#if defined(__linux__)
      //  }
#endif

        memset (macStr, 0, sizeof (macStr));
        if (u && u[0] + u[1] + u[2] + u[3] + u[4] + u[5])
        {
            sprintf (macStr, "%2.2X-%2.2X-%2.2X-%2.2X-%2.2X-%2.2X", u[0], u[1], u[2], u[3], u[4], u[5]);
            printf ("HW Address: %s", macStr);
        }

        printf("\n");


        ++ifr;
    }

    close(sockfd);
}
