APP=ld_cpp
INTC=interceptor
CC=g++
CC_FLAGS:= -Wall -g -O2

all: clean comp run

clean:
	rm -rf ${APP}.exe

comp:
	${CC} ${CC_FLAGS} ${APP}.cpp -o ${APP}.exe
	${CC} -Wall -shared -fpic  ${INTC}.cpp -o ${INTC}.so
run: comp
	./${APP}.exe