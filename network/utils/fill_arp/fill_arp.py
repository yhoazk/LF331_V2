#!/usr/bin/env python3

"""
Clear the arp table
ip -s -s neigh flush all
"""

def print_cmd(lim = 148, iface = "eth0"):
    for n in range(2,lim):
        ip_addr = n
        mac_addr = hex(n)[2:]
        print( f"ip neigh add 192.168.2.{ip_addr} lladdr 00:11:22:33:44:{mac_addr} dev {iface} nud reachable" )

if __name__ == "__main__":
    print_cmd()