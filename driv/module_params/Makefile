obj-m += mod_params.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# insmod mod_params.ko dev_names=name1,name2
# lsmod | grep mod_params
# dmesg  | tail -1
# rmmod hello_world_mod
