
#include <std/io.pat>
#include <std/mem.pat>
#include <std/sys.pat>

struct bootloader_descriptor {
    
    u32 boot_address[[comment("Address of start"), name("Bootloader start"), color("ff0000ff")]];
    u32 boot_size;  // size in bytes
    u32 boot_checksum;
    // At this point the struct values can be used
    calc_checksum(boot_address, boot_size);
};

u32 checksum;
fn calc_checksum(u32 start, u32 size) {
	//for (u8 i = 0, $+i < std::mem::read_unsigned(0x00, 1), i = i + 1) {
	for (u8 i = 0, i < size/4, i = i + 1) {
			checksum ^= std::mem::read_unsigned(start+(i*4), 4);
	}
	std::print("sdfsdf {:x}", checksum);
};

u32 magic1 @ 0x10;
std::assert(magic1 == 0xDEADBEEF, "incorrect value for magic1");
u32 magic2 @ 0x100;
std::assert(magic2 == 0xBADC0FFE, "incorrect value for magic2");
bootloader_descriptor boot @ 0x200;
std::assert(boot.boot_checksum == checksum, "Incorrect checksum");
// https://docs.werwolv.net/pattern-language/