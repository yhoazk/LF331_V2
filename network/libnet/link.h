#ifndef LIBNET_LINK
#define LIBNET_LINK
class link : public ioperand {
    struct  ifinfomsg *ifi;
    bool create();
    void read();
    void remove();
    uint8_t* dump(uint8_t* buf);
    uint8_t* build(uint8_t* buf);
};
#endif /* LIBNET_LINK */