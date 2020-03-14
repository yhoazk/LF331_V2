

#include "ioperand.h"
class route : public ioperand {
    struct rtmsg* rt;
    bool create() override {
        std::cout << "route create\n";
        return true;
    }
    void read() override {
        std::cout << "route read\n";
    }

    void remove() override {
        std::cout << "route remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
