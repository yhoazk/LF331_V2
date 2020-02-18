

#include "ioperand.h"
class netclass : public ioperand {
    struct tcmsg* tc_msg;

    bool create() override {
        std::cout << "class create\n";
        return true;
    }
    void read() override {
        std::cout << "class read\n";
    }

    void remove() override {
        std::cout << "class remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
