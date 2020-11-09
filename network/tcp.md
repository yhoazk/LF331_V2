

FIN vs RST

when the two parts of the communication end the conversation then there should be 
fin, ack
fin, ack
ack

But when some part in the connection is not fine, a RST is send.



MTU vs MSS:

Ethernet MTU: Maximum Transmission Unit

The MSS value normally is 1460 so it can accomodate 20 bytes of TCP header and 20 of eth header to get a total of 1500 to fit in the mtu


calculated windows size is the size of the buffer on one end. It is the result of multiply the window size with the option window size scaling factor.
The scaling factor is agreed during the handshake, the if this is not included in the trace the window might look too small, but it could be
that the actual window is being scaled.

This window says the number of bytes which can be sent w/o an ack
