SECTIONS {
    main 0x08000000 : {
        LONG(0x20000000 + 20K);
        LONG(reset_exception_handler | 1);
        . = 0x130;
        blink.o(code)
    }
}
