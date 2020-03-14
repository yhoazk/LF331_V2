#include <linux/rtnetlink.h>

#include "ioperand.h"

class link : public ioperand {
    enum class iface_type : unsigned short {
        WTF
    };
    int index; // local index
    bool up;
    bool loopback;
    bool running;
    bool noarp;
    bool promisc;
    
    struct  ifinfomsg *ifi;
    bool create() override {
        std::cout << "Link create\n";
        return true;
    }
    
    void read() override {
        std::cout << "Link read\n";
    }

    void remove() override {
        std::cout << "Link remove\n";
    }

    uint8_t* dump(uint8_t* buf) override {
        return buf;
    }
    uint8_t* build(uint8_t* buf) override {
        return buf;
    }
    link() = default;
    ~link() = default;
    private:
};
