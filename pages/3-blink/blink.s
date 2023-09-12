.cpu cortex-m3
.syntax unified
.thumb

.global reset_exception_handler

.section code

reset_exception_handler:

# enable I/O port C clock
ldr r0, =0x40021000 + 0x18
ldr r1, [r0]
orr r1, r1, 0x00000010
str r1, [r0]

# configure PC13 as open-drain output with 10 MHz speed
ldr r0, =0x40011000 + 0x04
ldr r1, [r0]
orr r1, r1, 0x00600000
bic r1, r1, 0x00900000
str r1, [r0]

blink_loop:

# wait loop
ldr r0, =1000000
delay_loop:
subs r0, r0, #1
bne delay_loop

# toggle PC13
ldr r0, =0x40011000 + 0x0c
ldr r1, [r0]
eor r1, r1, 0x00002000
str r1, [r0]

b blink_loop
