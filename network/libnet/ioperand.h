#ifndef LIBNETLINK_IOPERAND
#define LIBNETLINK_IOPERAND

#include <iostream>
#include <linux/rtnetlink.h>

class ioperand {
    public:
    virtual ~ioperand() {}
    virtual bool create() = 0;
    virtual void read() = 0;
    virtual void remove() = 0;
    virtual uint8_t* dump(uint8_t* buf) = 0; // write the contents on the buffer, return the last position
    virtual uint8_t* build(uint8_t* buf) = 0;  // read from buffer which contains an object and construct an object of the operand type
};

#endif /* LIBNETLINK_IOPERAND */
