#include "ioperand.h"


class neigh : public ioperand {
    struct ndmsg* nd;
    struct nda_cacheinfo* nda_cache;

    bool create() override {
        std::cout << "neigh create\n";
        return true;
    }
    void read() override {
        std::cout << "neigh read\n";
    }

    void remove() override {
        std::cout << "neigh remove\n";
    }
    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
};
