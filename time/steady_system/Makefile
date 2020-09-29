all: clean comp run

clean:
	rm -rf ./*.exe

comp:
	gcc updater.c -o updater.exe
	g++ cv_tester.cpp -o cv_tester_libc.exe -std=c++14 -lpthread -DLIBC_TEST
	g++ cv_tester.cpp -o cv_tester_boost.exe -std=c++14 -lpthread -DBOOST_TEST

run:
	./updater.exe &
	./cv_tester_libc.exe
