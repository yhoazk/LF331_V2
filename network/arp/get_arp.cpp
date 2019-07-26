// https://stackoverflow.com/questions/37371364/monitoring-arp-table-changes

#include <bits/stdc++.h>
#include <net/if.h>
#include <linux/if_link.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <libnetlink.h>

int main(){
  int sock;
  static struct sockaddr_nl g_addr;

  /* Zeroing addr */
  bzero(&g_addr, sizeof(g_addr));
  g_addr.nl_family = AF_NETLINK;
  g_addr.nl_groups = nl_mgrp(RTNLGRP_NEIGH);

  if ((sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
    printf("socket() error: %s", strerror(errno));
    return -1;
  }

  if (bind(sock, (struct sockaddr *) &g_addr, sizeof(g_addr)) < 0) {
    printf("bind() error: %s", strerror(errno));
    return -1;
  }

  char buffer[4096];
  int received_bytes = 0;

  while (true) {
    received_bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (received_bytes > 0) {
        printf("Event\n");
        // How to parse the event
    }
  }
  return 0;
}