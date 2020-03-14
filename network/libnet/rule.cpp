

#include "ioperand.h"

class rule : public ioperand {
    struct tcmsg* tc_msg;

    bool create() override {
        std::cout << "rule create\n";
        return true;
    }
    void read() override {
        std::cout << "rule read\n";
    }

    void remove() override {
        std::cout << "rule remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
