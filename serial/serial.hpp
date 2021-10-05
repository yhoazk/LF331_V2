#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <iostream>


extern "C" {
    #include <termios.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
}


struct serial_configuration {
    int k;
};


class serial_interface {
    public:
    explicit serial_interface(const std::string& dev);
    virtual bool config(const serial_configuration&);
    virtual size_t receive(std::vector<std::uint8_t>&);
    virtual size_t send(const std::vector<std::int8_t>&);
    virtual void close();
};


class serial {
private:
    const std::string dev_name;
    // serial_configuration port_config;
    int port;
public:
    explicit serial(const std::string& dev):dev_name{dev}, port{-1} {
        port = open(dev.c_str(), O_RDWR);
        if (port < 0) {
            std::cerr << "Error opening " << dev << std::strerror(errno) << std::endl;
        } else {
            std::cout << "Success opening: " << dev << std::endl;
        }

    }

    ~serial(){
        if (port != -1) {
            std::cout << "Closing port: " << dev_name << std::endl;
            ::close(port);
        }
    }

    bool config(const serial_configuration& config) {
        struct termios tty{};
        if (port == -1) {
            std::cerr << "Serial port handler is in incorrect state" << std::endl;
            return false;
        }

        tty.c_cflag &= ~PARENB; // disable parity chec
        tty.c_cflag &= ~CSTOPB; // Clear stio field only oe stop bit
        tty.c_cflag |= CS8;     // 8 bits per byte
        tty.c_cflag &= ~CRTSCTS; // disable RTS/CTS HW flow control
        tty.c_cflag |= CREAD | CLOCAL; // turn on read and ignore control lines
        // unix provides two basic modes of input:
        // - canonical: Input is processed when a new line char is received
        // - non-canonical: ?
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO; // disable echo
        tty.c_lflag &= ~ECHOE; // disable erasure
        tty.c_lflag &= ~ECHONL; // diable new line echo

        tty.c_lflag &= ~ISIG;

        tty.c_iflag &= ~(IXOFF | IXON | IXANY);
        // disable any special handling of reeived bytes
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // disable

        // output modes
        tty.c_oflag &= ~OPOST;        
        tty.c_oflag &= ~ONLCR;        
        tty.c_cc[VTIME] = 10;
        tty.c_cc[VMIN] = 0;

        // setting the baud rate
        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);
        ioctl(port, TCSETS, &tty);
    }

    size_t send(std::vector<std::uint8_t>& buf){
        return write(port, buf.data(), buf.size());
    }
};