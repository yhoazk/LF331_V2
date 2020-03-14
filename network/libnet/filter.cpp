
#include "ioperand.h"

class filter : public ioperand {
    struct tcmsg* tc_msg;

    bool create() override {
        std::cout << "filter create\n";
        return true;
    }
    void read() override {
        std::cout << "filter read\n";
    }

    void remove() override {
        std::cout << "filter remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
