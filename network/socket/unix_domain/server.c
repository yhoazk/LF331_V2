#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h> // contains the definition of the unix socket
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char** argv){
    static struct sockaddr_un server_addr;
    int fd,cl,rc;
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/mysock", sizeof(server_addr.sun_path)-1);
    bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    char buf[100];
    if(listen(fd, 5) == -1){
        perror("Listen:");
        exit(-1);
    }

    for(;;){
        if((cl = accept(fd, NULL, NULL)) == -1){
            perror("accept:");
            continue;
        }
        while ((rc = read(cl, buf, sizeof(buf))) > 0){
            // note the notation on the second arguemnt, here it says taht the number of chars to be added to the string shoul be taken from the list of parameters
            printf("Read %u bytes: %.*s\n", rc, rc, buf);
        }
        if(rc == -1){
            perror("read:");
        } else if (rc == 0){
            printf("eof\n");
            close(cl);
        }
        
    }
    return argc;
}