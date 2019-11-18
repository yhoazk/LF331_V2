#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>


int main(int argc, char** argv){
    struct sockaddr_un client_addr;
    char buf[100];
    int fd, rc;

    //if((fd = socket(AF_UNIX, SOCK_STREAM, 0) == -1)){ <<- this version does not assign the value to fd
    if( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("Socket error:");
        exit(-1);
    } else {
        printf("FD: %d\n", fd);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(&client_addr.sun_path, "/tmp/mysock", sizeof(client_addr.sun_path)-1);

    if(connect(fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1){
        perror("connect:");
        exit(-1); // needed to close the open socket connection
    }

    while( (rc = read(STDIN_FILENO, buf, sizeof(buf))) > 0 ){
        if(write(fd, buf, rc) != rc){
            if(rc > 0) fprintf(stderr, "partial write");
            else{
                perror("write error");
                exit(-1);
            }
        }
    }

    return argc;
}