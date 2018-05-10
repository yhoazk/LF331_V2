APP=hpet

all: clean comp run

clean:
	rm -rf ${APP}.exe

comp:
	gcc -Wall -o ${APP}.exe ${APP}.c

run:
	sudo ./${APP}.exe 33 33
