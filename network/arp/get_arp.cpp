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


bool process_arp_entry(string& line, ip_t& ip, mac_t& mac, array<uint8_t, 16>& ifname){
  string tok;
  stringstream l{line};
  vector<string> arp_tokens;
  while(getline(l, tok, ' ')){
    if(tok.length() > 1){
        cout << "x: " << tok << '\n';
        arp_tokens.push_back(tok);
    }
  }

  stringstream ip_str{arp_tokens[0]};
  cout << "ip str" << arp_tokens[0].c_str() << '\n';
  auto ip_it = std::begin(ip);
  while(getline(ip_str, tok, '.')){
    cout << tok << ',' << '\n';
    *ip_it++ = static_cast<uint8_t>(std::stoi(tok));
  }

  stringstream mac_str{arp_tokens[3]};
  cout << "mac str" << arp_tokens[3].c_str() << '\n';
  auto mac_it = std::begin(mac);
  while(getline(mac_str, tok, ':')){
    int x;
    istringstream(tok) >> std::hex >> x;
    cout << tok << '-' << x <<'\n';
    *mac_it++ = x;
  }

  std::copy(std::begin(arp_tokens[4]), std::end(arp_tokens[4]), std::begin(ifname)); 

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
      cout << line << '\n';
      process_arp_entry(line, ip, mac, iface_name);
      cout << "name: " << iface_name.data() << "ip " << std::hex << ip.data() << '\n';
      arpt[iface_name].push_back(std::make_pair(ip, mac));
    }
    cout << "Size of table: " << arpt.size() << '\n';

}


int main(){
  std::array<uint8_t, 16> name;
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