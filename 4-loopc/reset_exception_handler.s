.cpu cortex-m3
.syntax unified
.thumb

.global _reset_exception_handler

.text

_reset_exception_handler:
// zero out .bss section
mov r0, #0
ldr r1, =_bss_start
ldr r2, =_bss_end

copy_bss_loop:
cmp r1, r2
bge end_copy_bss
str r0, [r1], #4
b copy_bss_loop
end_copy_bss:

// copy .data section from flash to sram
ldr r0, =_flash_data_start
ldr r1, =_sram_data_start
ldr r2, =_sram_data_end

copy_data_loop:
cmp r1, r2
bge end_copy
ldr r3, [r0], #4
str r3, [r1], #4
b copy_data_loop

end_copy:
b start
