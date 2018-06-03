
all: clean comp run

clean:
	rm -rf server.exe client.exe

comp:
	gcc -o server.exe server.c -lrt
	gcc -o client.exe client.c -lrt

run:
	./server.exe &
	./client.exe
