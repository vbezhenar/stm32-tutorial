.cpu cortex-m3
.syntax unified
.thumb

.global _reset_exception_handler

.text

_reset_exception_handler:
ldr r0, =_flash_data_start
ldr r1, =_sram_data_start
ldr r2, =_sram_data_end

copy_loop:
cmp r1, r2
bge end_copy
ldr r3, [r0], #4
str r3, [r1], #4
bl copy_loop
b .

end_copy:
b start
