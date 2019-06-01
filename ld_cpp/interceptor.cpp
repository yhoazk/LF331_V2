#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

// detect the current TTY being used by the terminal executing this file
// open the termianl and write a test string
// clone the fd of the terminal and replace the calls to the syslog fs

// this will be the dynamically linked library
namespace somenamespace {

    int log_to_tty(char* str, int size){
        write(1, str, size);
        return size;
    }
}