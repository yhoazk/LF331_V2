#!/usr/bin/env python3

from scapy.all import IP, ICMP

def bcast_icmp():
    ip = IP(dst="160.48.199.22", src="160.48.199.127")
    icmp = ICMP(type=8, id=2)
    p = Ether(src="11:22:22:33:2b:ef", dst="11:22:22:33:2b:ef")/ip/icmp
    sendp(p, iface="virbr0")

if __name__ == "__main__":
    bcast_icmp()
