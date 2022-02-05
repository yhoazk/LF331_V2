APP:=offset_clk

all: clean test

clean:
	rm -rf ${APP} test_${APP}

test_comp:
	g++ ${APP}.h ${APP}.cpp test_${APP}.cpp -o test_${APP} -lgtest -lgtest_main

test: test_comp
	./test_${APP}