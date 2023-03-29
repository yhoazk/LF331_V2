
Linux feature allowing to isolate network environments through virtualization.
With this separate network interfaces and routing tables can be created and
they will be isolated from the rest of the system and operate independently.

Linux implements 6 namespaces:

- pid
- net
- uts
- mnt
- ipc
- user

## Show current namespaces

```
lsns
```

### Add a network namespace

```
ip netns add test_ns # Add a network namespace
ip netns             # show available network namespaces
```

### Executing orders in an specific network namespace

```
ip netns exec <ns_name> <command>
```

### Adding interfaces in the namespace

```
# Create a 
ip link add v-enp2s0 type veth peer name v-eth0 
```
