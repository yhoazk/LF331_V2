#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <ctype.h>

#define ETHTOOL_GSET    0x1
#define SIOCETHTOOL     0x8946

/* ioctl(SIOCIFETHTOOL) GSET ("get settings") structure */
struct ethtool_cmd {                                                            
    uint32_t    cmd;
    uint32_t    supported;      /* Features this interface supports */
    uint32_t    advertising;    /* Features this interface advertises */
    uint16_t    speed;          /* The forced speed, 10Mb, 100Mb, gigabit */
    uint8_t     duplex;         /* Duplex, half or full */
    uint8_t     port;           /* Which connector port */
    uint8_t     phy_address;
    uint8_t     transceiver;    /* Which tranceiver to use */
    uint8_t     autoneg;        /* Enable or disable autonegotiation */
    uint32_t    maxtxpkt;       /* Tx pkts before generating tx int */
    uint32_t    maxrxpkt;       /* Rx pkts before generating rx int */
    uint32_t    reserved[4];
};      

int
refresh_inet_socket()
{
    static int netfd = -1;
    if (netfd < 0)
        netfd = socket(AF_INET, SOCK_DGRAM, 0);
    return netfd;
}

int refresh_net_dev_ioctl_speed_duplex(char *name)
{        
    struct ethtool_cmd ecmd;
    struct ifreq ifr;
    int fd;
         
    memset(&ecmd, 0, sizeof(ecmd));
    memset(&ifr, 0, sizeof(ifr));
    if ((fd = refresh_inet_socket()) < 0)
        return 0;
         
    /* ETHTOOL ioctl -> non-root permissions issues for old kernels */
    ecmd.cmd = ETHTOOL_GSET;
    ifr.ifr_data = (caddr_t)&ecmd;
    strncpy(ifr.ifr_name, name, IF_NAMESIZE);
    ifr.ifr_name[IF_NAMESIZE-1] = '\0';
    if (!(ioctl(fd, SIOCETHTOOL, &ifr) < 0)) {
        fprintf(stderr, "speed ioctl val \"%li\"\n", ecmd.speed);
        fprintf(stderr, "duplex ioctl val \"%li\"\n", ecmd.duplex + 1);
        return 1;
    }    
    return -1;
}        

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Use: %s <interface>\n", argv[0]);
		return -1;
	}
	refresh_net_dev_ioctl_speed_duplex(argv[1]);	
	return 0;
}
