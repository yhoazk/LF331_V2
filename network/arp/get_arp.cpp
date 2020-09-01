// https://stackoverflow.com/questions/37371364/monitoring-arp-table-changes

// add a new arp entry arp -i eth0 -s 192.168.0.4 00:50:cc:44:55:55

extern "C" {
#include <net/if.h>
#include <linux/if_link.h>
#include <linux/types.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <unistd.h>
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
// the ioctl SIOCGIFCONF returns only the interaces that are connected
// because is calling the L3 config

using namespace std;
using ip_t = std::array<uint8_t, 4>;
constexpr size_t ip_size = std::tuple_size<ip_t>::value;
using mac_t = std::array<uint8_t, 6>;
constexpr size_t mac_size = std::tuple_size<mac_t>::value;
using arp_entry = std::pair<ip_t,mac_t>;
using arp_table_t = map<std::array<uint8_t, 16>, vector<arp_entry>>;

class fd_guard{
  public:
  explicit fd_guard(const int fd) : m_fd{fd} {}

  fd_guard(const fd_guard&) = delete;
  fd_guard(fd_guard&&) = delete;
  fd_guard& operator=(const fd_guard&) = delete;
  fd_guard& operator=(fd_guard&&) = delete;

  operator bool() const{
    return m_fd != -1;
  }

  int get() {
    return m_fd;
  }

  ~fd_guard() { if(m_fd != -1) { close(m_fd); }}
  private: 
  int m_fd;
};


template<typename objtype, typename itertype>
itertype copy_to_buffer(const objtype& obj, itertype it ) {
  const void* ptr_obj = static_cast<const void*>(&obj);
  const uint8_t* dst = reinterpret_cast<const uint8_t*>(ptr_obj);

  const size_t len{sizeof(obj)};

  return std::copy(dst, dst+len,  it);
}

bool send_arp(const std::string& iface, const arp_entry& src, const arp_entry& dst) {
  // create sock
  using namespace std;
  sockaddr_ll llsock{0};
  llsock.sll_family = AF_PACKET;
  int idx = if_nametoindex(iface.c_str());
  if(idx != 0) {
    llsock.sll_ifindex = idx;
  } else {
    return false;
  }
  llsock.sll_halen = ip_size;
  std::fill(begin(llsock.sll_addr), begin(llsock.sll_addr) + mac_size, 0x33);

  int skt = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if(skt < 0) {
    perror("socket");
    return false;
  }

  fd_guard sock_fd{skt};
  int nop;
  if (setsockopt(sock_fd.get(), SOL_SOCKET, SO_BROADCAST, &nop, sizeof(nop)) == -1) {
    perror("setsocktopt");
    return false;
  }

  // fill data 
  ethhdr ethh{};
  arphdr arph{};
  
  std::fill(begin(ethh.h_dest), begin(ethh.h_dest) + mac_size, 0xff);
  std::copy(begin(src.second), end(src.second), ethh.h_source);

  ethh.h_proto = htons(ETH_P_ARP);

  arph.ar_hrd = htons(ARPHRD_ETHER);
  arph.ar_pro = htons(ETH_P_IP);
  arph.ar_hln = 6; // size of header
  arph.ar_pln = 4; // size of proto
  arph.ar_op = htons(ARPOP_REQUEST);

  // construct message
  std::array<uint8_t, 128> arp_pkt;
  auto arp_it = std::begin(arp_pkt);
  auto arp_it_begin = std::begin(arp_pkt);

  arp_it = copy_to_buffer(ethh, arp_it);
  arp_it = copy_to_buffer(arph, arp_it);
  // copy sender
  arp_it = copy_to_buffer(src.second, arp_it);
  arp_it = copy_to_buffer(src.first, arp_it);
  // copy tgr
  arp_it = copy_to_buffer(dst.second, arp_it);
  arp_it = copy_to_buffer(dst.first, arp_it);
  std::cerr << "ready to send\n";
  // send
  unsigned int socklen{sizeof(llsock)};
  int result = sendto(sock_fd.get(), arp_pkt.data(), arp_it - arp_it_begin, 0, (const sockaddr*)(&llsock), socklen);
  // wait for ans
}

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
  // fetch_arp();

  arp_entry src{{0, 0, 0, 0}, {0x0a, 0xC0, 0x02, 0xAA, 0xBB, 0xCC}};
  arp_entry dst{{192, 168, 0, 1}, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

  send_arp("wlp8s0", src, dst);

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