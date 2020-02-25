// https://stackoverflow.com/questions/37371364/monitoring-arp-table-changes

// add a new arp entry arp -i eth0 -s 192.168.0.4 00:50:cc:44:55:55

// #include <bits/stdc++.h>
// #include <net/if.h>
// #include <linux/if_link.h>
// #include <ifaddrs.h>
// #include <arpa/inet.h>
// // #include <linux/rtnetlink.h>
// #include <linux/netlink.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <algorithm>
// the ioctl SIOCGIFCONF returns only the interaces that are connected
// because is calling the L3 config

using namespace std;
using ip_t = std::array<uint8_t, 4>;
using mac_t = std::array<uint8_t, 6>;
using arp_entry = std::pair<ip_t,mac_t>;
using arp_table_t = map<std::array<uint8_t, 16>, vector<arp_entry>>;


void process_arp_entry(string& line, ip_t& ip, mac_t& mac, array<uint8_t, 16>& ifname){

  /* Clear the dst arrays */
  ip.fill(0);
  mac.fill(0);
  ifname.fill(0);

  string tok;
  stringstream ss{line};
  vector<string> arp_tokens;

  while(getline(ss, tok, ' ')){
    // the arp mask is normally '*'. But if is not, we should ignore it and that's why
    // we include it in the tokens, if we remove the = from the comparison the name
    // will be in position 4 in the vector of tokens
    if(tok.length() >= 1){
        arp_tokens.push_back(tok);
    }
  }

  stringstream ip_str{arp_tokens[0]};
  auto ip_it = begin(ip);
  while(getline(ip_str, tok, '.')){
    *ip_it++ = static_cast<uint8_t>(stoi(tok));
  }

  stringstream mac_str{arp_tokens[3]};
  auto mac_it = begin(mac);
  while(getline(mac_str, tok, ':')){
    int _octet; // using this var to force the int version of the >> operand
    istringstream(tok) >> hex >> _octet;
    *mac_it++ = static_cast<uint8_t>(_octet);
  }

  copy(begin(arp_tokens[5]), end(arp_tokens[5]), begin(ifname)); 

}

void fetch_arp(){
  ifstream arp_table("/proc/net/arp");
  arp_table_t arpt;
  string line;
  ip_t ip;
  mac_t mac;
  array<uint8_t, 16> iface_name;
  getline(arp_table, line); // drop the header
  while(getline(arp_table, line)){
    // cout << line << '\n';
    process_arp_entry(line, ip, mac, iface_name);
    cout << "name: " << iface_name.data() << '\n';
    arpt[iface_name].push_back(std::make_pair(ip, mac));
  }
  cout << "Size of table: " << arpt.size() << '\n';
  for (auto &&d : arpt) {
    cout << "iface name: " << d.first.data() << "  Entries: " << d.second.size() << '\n';
    // for (auto &&v : d.second) {
    //   cout << "ip: "  << v.first.data() << '\n';
    // }

  }
}

int main(){
  fetch_arp();
  return 0;
}

#if 0
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
#endif


// #include <algorithm>
// #include <fstream>
// #include <iomanip>
// #include <iostream>
// #include <sstream>
// #include <string>

// using namespace std;

// int main()
// {    
//     ifstream testFile("testdates.txt");    
//     string line;

//     while(getline(testFile, line)){

//         string date;
//         int time;
//         float amount;

//         std::replace(line.begin(), line.end(), ',', ' ');

//         stringstream ss(line);

//         ss >> date;
//         ss >> time;
//         ss >> amount;

//         cout << "Date: " << date << " ";
//         cout << "Time: " << std::setfill('0') << std::setw(4) << time << " ";
//         cout << "Amount: " << amount << " ";

//         cout << '\n';
//     }   
// }