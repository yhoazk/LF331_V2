APP:=clock_back

# for this to work the clock needs to be disabled in RPi
# systemctl stop systemd-timesyncd.service
# https://unix.stackexchange.com/questions/549149/how-does-linux-use-a-real-time-clock
# NTP also needs to be disabled to use the timedatectl command:
# timedatectl set-ntp false
all: clean comp run

clean:
	rm -rf ${APP}

comp:
	g++ -Wall -Wextra -lpthread -o ${APP} ${APP}.cpp

run:
	./${APP}
