#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>


int main(){
    int lds = socket(AF_UNIX, SOCK_STREAM ,0);
    return 0;
}