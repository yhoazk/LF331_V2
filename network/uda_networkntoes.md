

__Interconnection: Narrow waist__

How interconnections is achieved with the principle named narrow waist:

Goal: Interconnect many existing networks, hide underlying technology from applications.
Every internet device must speak IP, aka network layer. This layer provides guarantees to the network above.
IN this case the network layer, at the same tiem teh network layer provides guarantess to the transport layer, one of those guarantees is the "end-to-end" connectivity.
THe essential point is that the network protocol uses a single protocol, which is IP.

The problem is, that as every device is running IP, making changes to this layer is very difficult as concensus is needed to keep the connectivity.

__Goals: Survivability__

Networks works even if some devices fail. This is achieved with:
- replication
- fate sharing:
    - Acceptable to loose state information if that entity itself is lost
    - For example the state information of a router, the routing tables
    - The device has to be lost to withstand the failure
    - Easier to engineering
- NAT violates the fate sharing principle

__GOals_ Heteregenety__

Supported tru TCP/IP as a monolithic transport where TCP provides the flow control and reliable delivery.
But not every application requires reliable communication, eg video, voice, also DNS does not requires reliable.
THen a best effort is enough, then there is no information about failured, performance etc.
This makes a design simple, but makes debug hard.

__Goals distributed management__

Addressing is handled by different orgs. Fore example in america ARIN, europo RIPE.
ALso for names the DNS protocol does not requires a central authority, the same for the protocol BGP for routing.

This allows for organic growth and stable management. But oin the down side there is no owner.

__What is missing?__
from the paper:
- Security
- availability
- mobility
- scaling

The topics mentioned in Clarks paper are:
- Heterogeneity
- interconnection
- sharing

__End to end argument__

End to end arguemtn in a nutshell:

> The function in question can completely and correctly be implemented only with the knowlege and help of the application standing __at the end points if the communication system__. Therefore, providing that questioned function as a feature of the comminication system itself is __not possible__. (sometimes an incomplete version of the function provided by the communication system may be useful as a performance enhancement).

Example applications of the end to end argument:

- Error handling in file transfer
- end to end encryption
- TXP/IP split in error handling

This argument could be resumed in: `The network is dumb, but the endpoints are intelligent`.

__File transfer__

For example to send a file from computer A to computer B. Computer A has to:
- Read file from disk
- Communication system sends the file
- Transmits packets over the network

On the receiving side, B hasto:
- Glue the file from the packets
- Write the result to disk

What can go wrong?
- WR to file
- Reassemble the file
- Communication system

Different options can be used to help to secure the transfer of the file. For example the integrity of each packet can be checked with a checksum. Then ack every packet. However any of this solutions are incomplete, and only the application can decide whether the file is correct or not. This does not mean that the stack should not take these options, but the final check has to be done at the application.

The ends may vary depending what the application is. Therefore to argue the end to end the endpoints need to be found.

__end to end argument violations__

Anyway the end to end argument is only an argument. Not a law or rule. Then there are different violations:
- NAT Network address translation
- VPN tunnels
- TCP splitting: Improve connection
- Spam: End user is considered to be the human
- P2P systems
- Caches

Which functions belong to the minimun dumb network?
- routing
- multicast
- QoS
- NAT

Some argue that end to end argument disallows to include some interesting funcitonality in the "dumb" network.


__violation: NAT__

The network address translation is used to communicate networks, for example a private network at home where the devices are assigned private IPs (192.168.0.0/16) communicate with the router which has a public IP of 68.112.5.23.
The packets traverses the router and then the router changes the IP to send the packet to the public network.
The router has a table with matchin IP and ports and maps the egress and ingress packets.

The violation to the end to end principle is in that the nodes in the private network are not globally addressable or routable and other devices are not able to initiate inbound connections with those devices behind the NAT.

There are various protocols to go around the NAT, for example: STUN.
This works on UDP enabled NAT devices, in this types of protocol. The device send a UDP message to create an entry in the NAT. Once created the global addres is now routable and the port is also known. This information can be obtained using DNS. It is also possible to manually configure these holes in the NAT device.


### Lesson 3

How packages are moved between networks.

__switching and bridging__

PRoblem:
- How hosts find each other on a subnet
- How subnets are interconnected

 __Bootstrapping: Network two hosts__

Lets assume that two hosts want to communicate and they are directly connected thru their ETH interfaces.
One method to send this would be to use the host name or the IP address of the host. But the eth needs to know the HW address (MAC) of the other device. Or to send a multicast.

But how does a host lean the MAC address of another host? R: **ARP**

__ARP: Address resolution protocol__
On ARP a host queris the owner of an IP via broadcast to any other host in the network.
```
who as 192.168.1.22?
```

ONly the particular host who owns the specific IP adrress being queried will answer the broadcast with an unicast reposnse with the MAC address.
With this response the sender host can start to build its ARP table, which contains a relationship between IP:MAC.
Now this host has the information to send pkgs to the device without requesting with ARP the next time.

What are the queries and responses in ARP?

* Query: broadcast asking about IP
* Response: Unicast with MAC address


__Interconnecting LANs with Hubs:__
HUBS are the simple form of interconnection, they do not exist anymore in modern networks because a switch is at the same price. HUBs creates a broadcast medium, where all the packets on the network are seen on every host. All packets are seen everywhere, this causes flooding, collisions, failures and increases latency.

__Switches: Traffic isolation__

Switches perform a sort of isolation by not broadcasting every packet to all nodes, instead the LAN gets partitioned in multiple lan segments. Then only sends the pkgs to the corresponding segments, for this requires a swtich table: 

Switch table: MAts destination MAC to output port in the switch.

__Learning swtiches__

This switches mantains a table of dst addresses and ports on a switch. When the swtich is started the table is empty and then the pkgs are simply flooded to all the network, the same as a HUB.

Once the pkg is responded, the switch gets the srcs address for this pkg and adds it to the table. Once the reply is also sent the switch now nows that initiator node A port mapping and then the response is not flooded, but also the switch will add the response src address to the table, now the node B is on the table.

However there's still need to flood some packages, for example ARP and broadcast. The problem is now to avoid loops in the network, which are wanted for redundancy. This could create broadcast storms.

Note that Switches mostly take the information from the SRC of the pkg, because is known that the node is there, and not from the destination, because the destination could not be reached.


__Spanning tree:__

The spanning tree protocol allows to create a tree, which by definition is a graph w/o loops where all nodes are included. Then when a pkg needs to be flooded, the switch will only flood on the nodes that are part of the spanning tree. This way the loops are avoided.

__Construct spanning tree:__
1. elect root, this is the switch with smalles ID
2. Then each switch mist exclude link if not on the shortedt path to root

**initiall** every node thinks it is the root node, then they run an election process to determine which has the smaller ID


__switches and routers__

Switches (layer 2 /eth):
- Automatically configuring
- forwarding tends to be fast
- limited in broadcast 

Routers (layer 3 / IP):
- Not restricted to spanning tree


__Buffer sizing__

Question: How much buffering do routerts/switches need?

Given a source and a destination which go tru a router and the bottle neck capacity is `C` in bits/sec and the path round trip delay is named as `2T` in sec. A buffer would need `Buffer = 2T * C`.

But this rule of thumb is not valid any more, because some times it increases the neede memory in the router affecting costs and also it delay the response for the anticongestion algorithms. 

__Buffer sizin for a TCP sendser__

Given the nature of TCP and its adjusting windows, the buffer size is not needed all the time at max capacity.
Then having different nodes with different TCP streams at different stages of the windows adjustement. We can conclude that the buffer size can be adjusted to `(2T * C / sqrt(n))` this by using the central limit theorem where `n` is the number of tcp streams, it is expected that the occupancy will approach a gaussian where it will become narrower at a `1/srqt(n)` rate. 


### Lesson 4: Routing

__Internet routing:__

