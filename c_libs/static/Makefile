APP=test_exe
all: clean get_obj get_lib link run

clean:
	rm -rf ./${APP}.exe *.o *.a

get_obj:
	cc -Wall -c ctest1.c ctest2.c

get_lib:
	ar -cvq libctest.a ctest1.o ctest2.o
link:
	cc -o ${APP}.exe prog.c libctest.a
run:
	./${APP}.exe
