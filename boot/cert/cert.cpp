#include <cstdint>
#include <cstring>
#include <array>
#include <bit>

#define BOOTLOADER_SIZE (0xf0)
// https://godbolt.org/z/cEbavrfsc
template <typename T, std::size_t SIZE>
constexpr uint32_t calculate_some_checksum(const std::array<T, SIZE> data) {
    uint32_t checksum{0};
    const std::size_t size = SIZE / sizeof(uint32_t);
    // Use an array of the same size, so bit_cast works
    std::array<uint32_t,size> data_as_int{std::bit_cast<std::array<uint32_t,size>>(data)};

    // std::memcpy(data_as_int.data(), data.data(), SIZE);

    // Since we used the same size for data_as_int as for data
    // the loop goes only to the actual data
    for (size_t i = 0; i < size; i++)
    {
        checksum ^= data_as_int[i];
    }
    return checksum;   
}

const unsigned int __attribute__ ((section (".magic_1"))) magic1 = 0xDEADBEEF;
const unsigned int __attribute__ ((section (".magic_2"))) magic2 = 0xBADC0FFE;
const unsigned int __attribute__ ((section (".magic_3"))) magic3 = 0xBADC0FFE;
const unsigned int __attribute__ ((section (".boot_addr"))) boot_addr = 0x00000300;
const unsigned int __attribute__ ((section (".boot_size"))) boot_size = BOOTLOADER_SIZE;
// variables do not need to be const
constexpr std::array<const uint8_t, BOOTLOADER_SIZE> __attribute__ ((section(".bootloader"))) bootloader = {0,1,2,3,4,5,6,7,8,9,'A','B','A','C','O'};
constexpr uint32_t __attribute__ ((section (".boot_check"))) boot_check = calculate_some_checksum(bootloader);

