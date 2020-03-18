
from scapy.all import *
import time

broadcast_net = "192.168.0.255"

arp_entries = {
        "192.168.0.2" : "00:11:22:33:44:22",
        "192.168.0.3" : "00:11:22:33:44:33",
        "192.168.0.4" : "00:11:22:33:44:44",
        }


local_mac = get_if_hwaddr("enp9s0")

def handle_pkg(pkg):
    print(pkg.show())
    if pkg[ARP].op == ARP.who-has:
        print(f"received arp request: {pkg.pdst}")
        if pkg.pdst in arp_entries:
            reply = ARP(op=ARP.is_at, hwsrc=arp_entries[pkg.pdst], psrc=pkg.pdst, hwdst="ff:ff:ff:ff:ff:ff", pdst=local_mac)
            go = Ether(dst="ff:ff:ff:ff:ff:ff", src=local_mac) / reply
            sendp(go)
    return


sniff(filter="arp", prn=handle_pkg, store=0)
