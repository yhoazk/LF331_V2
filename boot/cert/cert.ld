SECTIONS {
    . = 0x0;
    .rodata : {
        . = 0x00000010;
        KEEP(*(.magic_1))
        . = 0x00000100;
        KEEP(*(.magic_2))
}
}
