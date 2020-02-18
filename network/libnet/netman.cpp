
#include <iostream>
#include <array>
#include <bits/types/struct_iovec.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>

#include "ioperand.h"
#include "addr.h"
#include "netclass.h"
#include "filter.h"
#include "ioperand.h"
#include "link.h"
#include "neigh.h"
#include "qdisc.h"
#include "route.h"
#include "rule.h"

/*

link:
    create: Add an interface
        receives an interface object
    read:   Get the information of the interface
        receives a name, an index or empty for all
    remove:
        receives a name, interface or index

addr:
    create: adds an ip to an interface
        receives an interface object
    read: Get the addresses of the interdace
        receives name, index or interface obj or empty for all
    remove
        receives name, interface or index
route:
    create: adds a route
        receives a route object with src and dst
    read: gets the routes
        table id,  empty for all
    remove: delete an entry of the route table 
        entry id, ip dst
neigh:
    create: Not needed
    read:   Get the neigh table 
        an ip for the addres or empty for a map ip::mac
    remove:
        an ip to remove specific or empty for all
rule:
    create: todo
    read: todo
    remove: todo
qdisc:
    create: todo
    read: todo
    remove: todo
class:
    create: todo
    read: todo
    remove: todo
filter:
    create: todo
    read: todo
    remove: todo

*/

class netman final{
private:
    int nlsock_fd; /* netlink socket */
    struct sockaddr_nl sock;
    struct iovec iov;
    std::array<uint8_t, 1024*8> buf{0};
public:
    netman(): nlsock_fd{0}{
        // connect
        memset(&sock, 0, sizeof(sock));
        nlsock_fd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
    }
    void create(ioperand& op) ;
    void read(ioperand& op);
    void remove(ioperand& op);
    bool apply();

    ~netman() {
        close(nlsock_fd);
    }
};

void netman::create(ioperand& op){
    op.create();
}



int main(){
    link ln;
    netman nm;
    nm.create(ln);
    return 0;
}
