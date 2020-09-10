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
struct udp_stat {
  size_t uid;
  size_t inode;
  size_t drops;
};

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


void process_udp_line(string& line, udp_stat& stat) {
  using namespace std;
  /* Clear the dst arrays */
  string tok;
  stringstream ss{line};
  vector<string> arp_tokens;

  while(getline(ss, tok, ' ')){
    if(tok.length() >= 1){
        arp_tokens.push_back(tok);
    }
  }
  
  // std::cout << "TOK len: " << std::to_string(arp_tokens.size()) << '\n';
  stat.inode = stoi(arp_tokens[9]);
  stat.drops = stoi(arp_tokens[12]);

}

void fetch_udp(){
  ifstream arp_table("/proc/net/udp");
  string line;

  getline(arp_table, line); // drop the header
  while(getline(arp_table, line)){
    udp_stat stat;
    process_udp_line(line, stat);
    cout << "inode: " << stat.inode << " drops: " << stat.drops << '\n';
  }
}

int main(){

  fetch_udp();  

  return 0;
}