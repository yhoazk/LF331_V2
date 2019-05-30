# Socket Types

The socket types are classified according to the communication properties.
Processes normally communicate between sockets of the same type, but sockets of
different type caon communicate.

The type of socker describes the semantics of the communication and also
describes the expected properties of the communication such as ordering,
reliability and prevention of duplicated msgs.

A list of supported sockets can be found in: `/usr/src/kernels/$(uname -r)/include/linux/socket.h`

## datagram
- `SOCK_DGRAM`
- Provides datagrams, which are connectionlesss messages of a
fixed max length.
- Order and reliability are not guaranteed
- In unix domain this type of socket is similar to a message queue
- In internet domain this type of socket  id implemented as UDP/IP
- Supports bidirectional flow of data
- The data is not sequenced, and the order may be different to the sent order
- Data is not unduplicated

## stream
- `SOCK_STREAM`
- The data is sequenced, ordered and Txd in reliable basis
- Two-way byte stream
- Mechanism for stream data
- Unix: pipe
- INET: TCP/IP
- Aside from the bidirectionality of data flow, a pair of connected stream
sockets provides an interface nearly identical to pipes
- This type of socket must be connected before the data transmission.
- These steps are needed for data Rx/Tx
  - create a connection to another socket with `connect`
  - Use the `read` and `write` subroutines to the `send` and `recv`
  - Use the `close` subroutine to finish the session
- If a piece of data for which the peer protocol has buffer space cannot be
successfully transmitted within a reasonable period of time, the connection
is broken. in that case the `socket` subroutine sets the `errno` to `ETIMEDOUT`
- When a process sends on a broken stream, a `SIGPIPE` signal is raised.
## Raw
- `SOCK_RAW`
- Provides access to internal network protocols and interfaces
- Available to root user or users with `CAP_NUMA_ATTACH` capability
- Allow applications to have direct access to low level communication protocols
- Normally datagram-oriented

## Sequential Packet
- `SOCK_SEQPACKET`
- Provides sequenced, reliable, and unduplicated flow of information

## Connection datagram
- `SOCK_CONN_DGRAM`
- Connection oriented datagram service
- Bi-directional flow of data
- Data is sequenced and unduplicated but not reliable
- Socket connection _must_ be prior ro data transfer
- Only supported by the Asynchronous Transfer Mode protocol and Network Devide Driver

The process group associated with a socket can be read or set by either the
`SIOCGPGRP` or `SIOCSPGRP` ioclt operation. To receive a signal on any data,
use both the `SIOCSPGRP` and `FIOASYNC` ioctl operations defined in `sys/ioclt.h`


## socket protocols

The file `/usr/include/sys/socket.h` contains a list of socket protocol families.
For example:

- `PF_UNIX` Local communication
- `PF_INET` Internet (TCP/IP)
- `PF_NDD` The operating system NDD

- - -

## Out of Band data
