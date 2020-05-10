



#### Notes:

`tcpdump` only sets the interfaces on promisc mode if only one interface is
selected, when any interface is selected, then tcpdump makes no changes in the
status of the interfaces, then only pkgs directed to that MAC address will be
visible.



### Sysfs net class


The counters in `/sys/class/net/*/statistics` are not cleared when the
interfaces go down, only removing the network drivers or a reboot will
restart the counters.

