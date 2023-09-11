MEMORY
{
    Flash : ORIGIN = 0x08000000, LENGTH = 64K
    SRAM : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS {
    .isr_vector :
    {
        LONG(0x20000000 + 20K);
        LONG(_startup | 1);
        . = 0x130;
    } > Flash

    .text :
    {
        . = ALIGN(4);
        *(.text)
        . = ALIGN(4);
    } > Flash

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        . = ALIGN(4);
    } > Flash
    
    .bss : {
        . = ALIGN(4);
        *(.bss)
        . = ALIGN(4);
    } > SRAM

    .data : {
        . = ALIGN(4);
        _flash_data_start = LOADADDR(.data);
        _sram_data_start = .;
        *(.data)
        _sram_data_end = .;
        . = ALIGN(4);
    } > SRAM AT> Flash
}
