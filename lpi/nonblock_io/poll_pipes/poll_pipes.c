#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>



int (*fifo_io)[2]; // pointer to an array of 2 ints

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage;\n%s <num of pipes>", argv[0]);
        exit(1);
    }
    int num_pipes = atoi(argv[1]);
    // create the space to hold the file descriptors of the input and output fifos
    fifo_io = calloc(num_pipes, sizeof(int[2]));
    while(num_pipes--){
        // pipe returns 2 file descriptors, fd[0] represents the read end fd[1] the write end.
        pipe(fifo_io[num_pipes]);
        // set the O_NONBLOCK option to the write part of the fifo
        fcntl(fifo_io[num_pipes][1], F_SETFL, fcntl(fifo_io[num_pipes][1], F_GETFL, 0) | O_NONBLOCK);
    }

    exit(0);
}