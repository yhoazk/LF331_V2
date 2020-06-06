



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
