# ICMP


## Ignore ICMP broadcasts

It is recommended to set the flag `icmp_echo_ignore_broadcasts`.

```
sysctl -w net.ipv4.icmp_echo_ignore_broadcasts=1
```

This is set by default, when it is set to `0` the host will respond which could be used for DoS.

## Error cases

This is not a error in the handling of the ICMP message, but a mismatch with calculated and actual broad cast address.
In the example, the interface was set to:

```
6: tap0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel master virbr0 state UNKNOWN group default qlen 1000
    link/ether fe:91:b9:61:7c:ed brd ff:ff:ff:ff:ff:ff
    inet 160.48.199.22/25 brd 160.48.199.255 scope global tap0
       valid_lft forever preferred_lft forever
    inet6 fe80::fc91:b9ff:fe61:7ced/64 scope link 
       valid_lft forever preferred_lft forever

```

There calculated broadcast is:

```
ipcalc 160.48.199.0/25
Network:	160.48.199.0/25
Netmask:	255.255.255.128 = 25
Broadcast:	160.48.199.127

Address space:	Internet
HostMin:	160.48.199.1
HostMax:	160.48.199.126
Hosts/Net:	126
```

If the setting is enabled it will check the broadcast address, not the calculated one. Then host will respond to broadcast if the ICMP comes
from the address `160.48.199.127`, which is the broadcast address for this interface according to the CIDR but not to the `ip` command.