SECTIONS {
    flash 0x08000000 : {
        LONG(0x20000000 + 20K);
        LONG(reset_exception_handler | 1);
        . = 0x130;
        blinkc.o(.text)
        blinkc.o(.rodata)
    }

    sram 0x20000000 : {
        blinkc.o(.data)
        blinkc.o(.bss)
    }
}
