# Linux Bridge


## Setting bridge `systemd`

### Create device

Create a file in: `/etc/systemd/network/<br_name>.netdev` and a configuration
for this bridge in the same folder `<br_name>.network`. 

```
cat <br_name>.netdev
[NetDev]
Name=br0
Kind=bridge
```

Once the file `netdev` is created, is possible to create the bridge by calling:
`systemctl start systemd-networkd.service`



### Configure device

## Bridge Monitoring

Which information is useful to determine the sw bridge health status?

- Forwarding status:
    - `/proc/sys/net/ipv4/ip_forward`
- State of interfaces:
    - `/sys/class/net/<dev>/operstate` E `{down, up}`
- Proxy arp state
    - `/proc/sys/net/ipv4/conf/<iface>/proxy_arp`
- Forwarding in interface
    - `proc/sys/net/ipv4/conf/<iface>/forwarding`
- Confirm the presence of the bridge
- Confirm the operstate of the bridge
    - `/sys/class/net/<bridge>/operstate` E `up, down`
- Confirm the correct state of the bridge



The command `bridge` includes an utility to monitor the bridge.

To add a bridge w/o `brctl` the command `ip` is used not the `bridge` cmd.

```
ip link add <name> type bridge
```

Add an interface to the bridge

```
ip link set <iface> master <bridge>
```


## forward_delay `/sys/class/net/<bridge>/bridge/forward_delay`


https://wiki.linuxfoundation.org/networking/bridge

## Consequences of activate a bridge



When a bridge is set in the system and the iterfaces atteched, the involved
interfaces are set to __promiscuous__ mode. Normally, the messages which have
a different mac address of the machine are discarded. This means that the
kernel does not copies the message which it seems are not directed to this
machine. When the `promisc` mode is activated all the messages are copied to
user space. This has a performance penalty (reference needed).

When the interfaces are attached tools like `ifconfig` and `ip` do not show
the PROMISC flag for some reason. The most reliable method to find the status
of the interface is to check the `flags` in the `sysfs`. 

```
cat /sys/class/net/<dev>/flags
0x1003 < here the interface is not in promisc mode
ip link set dev <dev> promisc on
cat /sys/class/net/<dev>/flags
0x1103 < promisc mode was activated
```

The description of every bit in the printed `int` are in the file `/usr/include/linux/if.h`
pasted here:

```c
/**
 * enum net_device_flags - &struct net_device flags
 *
 * These are the &struct net_device flags, they can be set by drivers, the
 * kernel and some can be triggered by userspace. Userspace can query and
 * set these flags using userspace utilities but there is also a sysfs
 * entry available for all dev flags which can be queried and set. These flags
 * are shared for all types of net_devices. The sysfs entries are available
 * via /sys/class/net/<dev>/flags. Flags which can be toggled through sysfs
 * are annotated below, note that only a few flags can be toggled and some
 * other flags are always preserved from the original net_device flags
 * even if you try to set them via sysfs. Flags which are always preserved
 * are kept under the flag grouping @IFF_VOLATILE. Flags which are __volatile__
 * are annotated below as such.
 *
 * You should have a pretty good reason to be extending these flags.
 *
 * @IFF_UP: interface is up. Can be toggled through sysfs.
 * @IFF_BROADCAST: broadcast address valid. Volatile.
 * @IFF_DEBUG: turn on debugging. Can be toggled through sysfs.
 * @IFF_LOOPBACK: is a loopback net. Volatile.
 * @IFF_POINTOPOINT: interface is has p-p link. Volatile.
 * @IFF_NOTRAILERS: avoid use of trailers. Can be toggled through sysfs.
 *	Volatile.
 * @IFF_RUNNING: interface RFC2863 OPER_UP. Volatile.
 * @IFF_NOARP: no ARP protocol. Can be toggled through sysfs. Volatile.
 * @IFF_PROMISC: receive all packets. Can be toggled through sysfs.
 * @IFF_ALLMULTI: receive all multicast packets. Can be toggled through
 *	sysfs.
 * @IFF_MASTER: master of a load balancer. Volatile.
 * @IFF_SLAVE: slave of a load balancer. Volatile.
 * @IFF_MULTICAST: Supports multicast. Can be toggled through sysfs.
 * @IFF_PORTSEL: can set media type. Can be toggled through sysfs.
 * @IFF_AUTOMEDIA: auto media select active. Can be toggled through sysfs.
 * @IFF_DYNAMIC: dialup device with changing addresses. Can be toggled
 *	through sysfs.
 * @IFF_LOWER_UP: driver signals L1 up. Volatile.
 * @IFF_DORMANT: driver signals dormant. Volatile.
 * @IFF_ECHO: echo sent packets. Volatile.
 */

enum net_device_flags {
/* for compatibility with glibc net/if.h */
#if __UAPI_DEF_IF_NET_DEVICE_FLAGS
	IFF_UP				= 1<<0,  /* sysfs */
	IFF_BROADCAST			= 1<<1,  /* __volatile__ */
	IFF_DEBUG			= 1<<2,  /* sysfs */
	IFF_LOOPBACK			= 1<<3,  /* __volatile__ */
	IFF_POINTOPOINT			= 1<<4,  /* __volatile__ */
	IFF_NOTRAILERS			= 1<<5,  /* sysfs */
	IFF_RUNNING			= 1<<6,  /* __volatile__ */
	IFF_NOARP			= 1<<7,  /* sysfs */
	IFF_PROMISC			= 1<<8,  /* sysfs */
	IFF_ALLMULTI			= 1<<9,  /* sysfs */
	IFF_MASTER			= 1<<10, /* __volatile__ */
	IFF_SLAVE			= 1<<11, /* __volatile__ */
	IFF_MULTICAST			= 1<<12, /* sysfs */
	IFF_PORTSEL			= 1<<13, /* sysfs */
	IFF_AUTOMEDIA			= 1<<14, /* sysfs */
	IFF_DYNAMIC			= 1<<15, /* sysfs */
#endif /* __UAPI_DEF_IF_NET_DEVICE_FLAGS */
#if __UAPI_DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO
	IFF_LOWER_UP			= 1<<16, /* __volatile__ */
	IFF_DORMANT			= 1<<17, /* __volatile__ */
	IFF_ECHO			= 1<<18, /* __volatile__ */
#endif /* __UAPI_DEF_IF_NET_DEVICE_FLAGS_LOWER_UP_DORMANT_ECHO */
};
```


