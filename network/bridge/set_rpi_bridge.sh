#!/usr/bin/sh


ip link add name br0 type bridge
ip link set br0 up
ip link set dev eth0 master br0
ip link set dev eth1 master br0

