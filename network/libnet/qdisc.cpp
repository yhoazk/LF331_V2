

#include "ioperand.h"
class qdisc : public ioperand {
    struct tcmsg* tc_msg;

    bool create() override {
        std::cout << "qdisc create\n";
        return true;
    }
    void read() override {
        std::cout << "qdisc read\n";
    }

    void remove() override {
        std::cout << "qdisc remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
