#include "ioperand.h"


class addr : public ioperand {
    struct ifaddrmsg* ifaddr;
    bool create() override {
        std::cout << "Addr create\n";
        return true;
    }
    void read() override {
        std::cout << "Addr read\n";
    }

    void remove() override {
        std::cout << "addr remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
