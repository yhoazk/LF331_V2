APP=print_errno


all: clean comp run

clean:
	rm -rf ${APP}

comp:
	gcc -Wall -o ${APP}.exe ${APP}.c

run:
	./${APP}.exe
