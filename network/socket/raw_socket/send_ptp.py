#!/usr/bin/env python2
from scapy.utils import rdpcap
from scapy.layers.l2 import Ether
from scapy.packet import Packet, bind_layers
from scapy.fields import *

class ieee1588(Packet):
  name = "Precision Time Protocol"
  fields_desc = [
    XBitField('transportSpecific', 0x1, 4),
    XBitField('messageType', 0x3, 4),
    XByteField('versionPTP', 0x05),
    XShortField('messageLength', 0x0036),
    XByteField('subdomainNumber', 0),
    XByteField('none1', 0),
    XShortField('flags', 0x0208),
    XBitField('correction', 0x00000000,64),
    XBitField('none2', 0x00,32),
    XBitField('ClockIdentity', 0x08028efffe9b97a5,64),
    XShortField('SourcePortId', 0x0002),
    XShortField('sequenceId', 0x0566),
    XByteField('control', 0x05),
    XByteField('logMessagePeriod', 0x7F),
    XBitField('requestreceiptTimestampSec', 0x00000000057b,48),
    XBitField('requestreceiptTimestampNanoSec', 0x0d11715c,32),
    XBitField('requestingSorucePortIdentity', 0x08028efffe9b97a5,64),
    XShortField('requestingSourcePortId', 0x0002)
  ]

bind_layers(Ether, ieee1588, type=0x88F7)
# bind_layers(Ether, ieee1722, type=0x22f0)
# bind_layers(ieee1722, iec61883)

pcap = rdpcap("./PTP_sync.pcap")

for pkt in pcap:
  ptp = pkt.getlayer(ieee1588)
  print(ptp.correction)
'''
#!/usr/bin/env python

from socket import *

#
# Ethernet Frame:
# [
#   [ Destination address, 6 bytes ]
#   [ Source address, 6 bytes      ]
#   [ Ethertype, 2 bytes           ]
#   [ Payload, 40 to 1500 bytes    ]
#   [ 32 bit CRC chcksum, 4 bytes  ]
# ]
#

s = socket(AF_PACKET, SOCK_RAW)
s.bind(("eth1", 0))
src_addr = "\x01\x02\x03\x04\x05\x06"
dst_addr = "\x01\x02\x03\x04\x05\x06"
payload = ("["*30)+"PAYLOAD"+("]"*30)
checksum = "\x00\x00\x00\x00"
ethertype = "\x08\x01"
s.send(dst_addr+src_addr+ethertype+payload+checksum)
'''