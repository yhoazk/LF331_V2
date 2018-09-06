APP=filetype

all: clean comp run

clean:
	@rm -rf ${APP}.exe

comp:
	@gcc -Wall -o ${APP}.exe ${APP}.c

run:
	@./${APP}.exe
