#### Notes:

`tcpdump` only sets the interfaces on promisc mode if only one interface is
selected, when any interface is selected, then tcpdump makes no changes in the
status of the interfaces, then only pkgs directed to that MAC address will be
visible.

### Sysfs net class

The counters in `/sys/class/net/*/statistics` are not cleared when the
interfaces go down, only removing the network drivers or a reboot will
restart the counters.


### eBPF and RT patch

RT patch and eBPF does not play along. Apparently there has not been an agreement
about which parts are to be disablen when eBPF and/or RT patch is included.


### when is necessary to bind a socket to an address?

For raw sockets, the connection cannot be determined with IP or the route table
then is necessary to specify which interface is to be used.

if the socket gets binded to `0.0.0.0` it listens to all interfaces.
If the socket it binded to a specific IP then it binds to the interface with that
specific address. If the connection is done to an unbound socket the interface to
which gets bounded is determined by the routing tables.

This bind occurs when specifically calling bind or when connect is called


### IPv4 vs IPv6 wrt Security

IPv6 now has a 128bit address while IPv4 has a 32bit.
Main security differences with IPv4 is that IPv6 can run end-to-end encryption.
This technology was retrofitted into IPv4. Integrity checks and encryption are
standard components of IPv6, which will make MITM attacks significantly more
difficult.

IPv6 also supports a more secure name-resolution protocol Secure Neighbor discovery
(SEND) protocol enables cryptographic confirmation that a host is who it claims to
be at connection time. Which will render ARP posioning and other naming-based attacks
more complicated, while not replacing application or service layer verification still
improves level of thrust. The most usual issue is a misconfiguration of either IPv4
or 6 and while the transition is done there would be need to support both, then
the need of transport IPv6 over IPv4, which will bring a new series of problems.


### Promiscuos

There are several levels of promiscuity in linux. Tools like `ip` and `ifconfig` do not detect promisc mode when is not explicitly requested, the driver decides when to enable promisc mode. For example, if the driver does not have enough granularity and we need to process PTP frames which go to a broadcast mac address, the driver changes to promisc mode.
`tcpdump` for example enables the mode `1`, there `ip` does not report it by default.

There are three levels in the promisc mode:

`0`: promisc mode disabled
`1`: promisc mode implicitly enabled
`2`: promisc mode explicitly enabled

In state `1` tools `ip` and `ifconfig` will not report it. Only `ip` will report it with the following options:
[src](https://unix.stackexchange.com/questions/561102/what-determines-an-interfaces-promiscuity-the-interface-flags-or-properties)
```
ip -d l l eth0
```

























