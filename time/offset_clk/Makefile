APP:=offset_clk

all: clean comp run

clean:
	rm -rf ${APP} test_${APP}

comp:
	g++ -Wall -Wextra -pedantic -o ${APP} ${APP}.cpp

run:
	./${APP}

test_comp:
	g++ ${APP}.h ${APP}.cpp test_${APP}.cpp -o test_${APP} -lgtest -lgtest_main

test: test_comp
	./test_${APP}