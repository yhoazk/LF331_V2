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
-``


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