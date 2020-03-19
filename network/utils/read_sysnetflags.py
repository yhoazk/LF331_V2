#!/usr/bin/env python

import os

sysfs_path = "/sys/class/net/"

def read_flags(flags_string):
    int_flags = int(flags_string[2:])

    print(f"Flag value {int_flags}")
    for k, v in flags.items():
        if int_flags & v:
            print(k)

def get_flags(dev_name=""):
    flag_val = []
    ifaces = os.listdir(sysfs_path)

    for iface in ifaces:
        with open(sysfs_path + iface + "/flags") as fd_iface:
            flag_val.append((iface, fd_iface.read()))


    return flag_val

# From https://github.com/torvalds/linux/blob/master/include/uapi/linux/if.h#L82
flags = {
    "IFF_UP"          : 1<<0,  # sysfs
    "IFF_BROADCAST"   : 1<<1,  # volatile
    "IFF_DEBUG"       : 1<<2,  # sysfs
    "IFF_LOOPBACK"    : 1<<3,  # volatile
    "IFF_POINTOPOINT" : 1<<4,  # volatile
    "IFF_NOTRAILERS"  : 1<<5,  # sysfs
    "IFF_RUNNING"     : 1<<6,  # volatile
    "IFF_NOARP"       : 1<<7,  # sysfs
    "IFF_PROMISC"     : 1<<8,  # sysfs
    "IFF_ALLMULTI"    : 1<<9,  # sysfs
    "IFF_MASTER"      : 1<<10, # volatile
    "IFF_SLAVE"       : 1<<11, # volatile
    "IFF_MULTICAST"   : 1<<12, # sysfs
    "IFF_PORTSEL"     : 1<<13, # sysfs
    "IFF_AUTOMEDIA"   : 1<<14, # sysfs
    "IFF_DYNAMIC"     : 1<<15, # sysfs
    "IFF_LOWER_UP"    : 1<<16, # volatile
    "IFF_DORMANT"     : 1<<17, # volatile
    "IFF_ECHO"        : 1<<18, # volatile
}


if __name__ == "__main__":
    devs = get_flags()
    for name, val in devs:
        print(f"\nInterface: {name}")
        read_flags(val)
