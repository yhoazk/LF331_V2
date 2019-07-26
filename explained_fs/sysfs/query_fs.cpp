#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/in.h>

//https://tools.ietf.org/html/rfc2863

bool checkEthConfForInterface(const char* interface) noexcept {
    if (!interface)
        return false;

    std::stringstream ss;
    ss << "cat /sys/class/net/" << interface << "/carrier";

    std::string cmd = ss.str();

    const bool isEthConf = system(cmd.c_str())  == 0;
    return isEthConf;
}

bool checkEthPluggedForInterface(const char* interface) noexcept {

    if (!interface) return false;

    std::stringstream ss;
    ss << "grep -q 1 /sys/class/net/" << interface << "/carrier";
    
    std::string cmd =  ss.str();

    const bool isEthCommOK = system(cmd.c_str()) ==  0; 
    return isEthCommOK;
}

//--------------------------

/**
 * Checks if the interface is up 
 */

bool isEthConfigForInterface(const char* interface) noexcept{
    if (nullptr == interface) return false;
    std::stringstream ss;
    std::ifstream iface_carrier;
    std::string operational_state;
    ss << "/sys/class/net/" << interface << "/operstate";
    iface_carrier.open(ss.str());
    if(iface_carrier.is_open()){
        iface_carrier >> operational_state;
        std::cout << operational_state << '\n';
    } else {
        std::cerr << "Erorr opeinign\n";
    }
    iface_carrier.close();
    return operational_state == "up";
}

// set a inotify for the interfacesx

bool isEthIfaceCfgd(std::string iface){
    struct ifreq ifr;
    int sock_fd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
    memset(&ifr, 0, sizeof(ifr));

    strcpy(ifr.ifr_name, iface.c_str());
    if(ioctl(sock_fd, SIOCGIFFLAGS, &ifr) < 0){
        perror("SIOCGIFFLAGS");
    }
    close(sock_fd);
    return !!(ifr.ifr_flags & IFF_UP);
}

bool isEthPluggedForInterface(std::string iface) noexcept {

    struct ifreq ifr;
    int sock_fd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
    memset(&ifr, 0, sizeof(ifr));

    strcpy(ifr.ifr_name, iface.c_str());
    if(ioctl(sock_fd, SIOCGIFFLAGS, &ifr) < 0){
        perror("SIOCGIFFLAGS");
    }
    close(sock_fd);
    return !!(ifr.ifr_flags & IFF_RUNNING);
}

int main(int argc, char const *argv[])
{
    const char iface_name[] = "enp7s0";
    if( checkEthConfForInterface(iface_name) != isEthConfigForInterface(iface_name)){ std::cerr << "Mis MAtch\n";  }
    if( checkEthConfForInterface(iface_name) != isEthIfaceCfgd(iface_name)){
        std::cerr << "Mis Match\n";
    } else {
        std::cerr << "Match\n";
    }

    if(isEthPluggedForInterface(iface_name) != checkEthPluggedForInterface(iface_name)){
        std::cerr << "Plugged Mismatch\n";
    } else {
        std::cerr << "Plugged Matches\n";
    }
    
       checkEthPluggedForInterface(iface_name);
    return 0;
}