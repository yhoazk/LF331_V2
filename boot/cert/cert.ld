SECTIONS {
    . = 0x0;
    .rodata : {
        . = 0x00000010;
        KEEP(*(.magic_1))
        . = 0x00000100;
        KEEP(*(.magic_2))
        . = 0x00000200;
        KEEP(*(.boot_addr))
        KEEP(*(.boot_size))
        KEEP(*(.boot_check))
        . = 0x00000300;
        KEEP(*(.bootloader))
        KEEP(*(.magic_3))
}
}
