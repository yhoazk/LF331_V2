#include <iostream>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

char buffer[127];
// this function will implement the same functionality as tty
// but only for emulated terminals, for serial terminals use
// ioctl TCGETS 
void get_terminal_name(char* buf, size_t size){
    size_t read_n;
    if ((read_n = readlink("/proc/self/fd/0", buf, size)) < 0){
        perror("Fail readlink for self fd");
    }
    // append null terminator
    buf[read_n] = '\0';
}

/**
 * 
 */
void send_to_tty(void){
    //struct stat sb;

    const char test[] = "XXXX Test text\n";
    // file descriptor of the terminal where the text will be redirected
    int pstty;
    // static name of the interface found with the command tty
    // const char* tty_name = "/dev/pts/0";
    // get the tty name from the function
    get_terminal_name(buffer, 127);
    // to find the tty w/o the command tty
    // read link to /proc/self/fd/0
    // Even when the fd are as described below, all refer to the same terminal
    // fd/0 = read
    // fd/1 = write
    // fd/2 = error
    // As the tty implements the open/read/write calls 
    // In linux the files ca be open by several processes, but there are no guarantees
    // about the result of the r/w operations. Locking is not enforced by default 
    if((pstty = open(buffer, O_RDWR)) < 0){
        perror("Error opening the terminal");
    } else {
        write(pstty, test, sizeof(test));
    }
    close(pstty);
}



int main(void){
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("ld_cpp", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    // this messages are to be logged into journal
    syslog(LOG_NOTICE, "Program started by user %d", getuid());
    syslog(LOG_INFO, "A tree falls in the forest");
    send_to_tty();
    
    closelog();
    return 0;
}