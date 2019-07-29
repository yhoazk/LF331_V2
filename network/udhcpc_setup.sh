#!/bin/sh -xe
# Inside the busybox ECU run:
# create a symbolic link to busybox with the name of the 
# applet, busybox binary reads the argv[0] to determine its
# behaviour ln -s /bin/busybox /usr/udhcpc
# udhcpc -n -q -t 2 -s /home/setup.sh

clear_route() {
  while route del default gw 0.0.0.0 $interface 2> /dev/null; do
   :
  done
}

default_ip() {
  echo '192.168.4.22'
}

case "$1" in
  leasefail|nak)
    /bin/ifconfig eth0 "$(default_ip)"
    clear_route
    ;;
  renew|bound)
    if [[ -n "$broadcast" ]]; then
      BROADCAST="broadcast $broadcast"
    fi
    if [[ -n "$subnet" ]]; then
      NETMASK="netmask $subnet"
    fi
    clear_route
    /bin/ifconfig eth0 $ip $BROADCAST $NETMASK
    echo $router
    if [[ -n "$router" && "$router" != "0.0.0.0" ]]; then
      for i in $router; do
        # here is the failure in the original set up
        route add default gw $i dev $interface
      done
    fi
    ;;
esac
