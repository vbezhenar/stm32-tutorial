.cpu cortex-m3
.syntax unified
.thumb

.global reset_exception_handler

.section code

reset_exception_handler:

// enable I/O port C clock
ldr r0, =0x40021000 + 0x18 // RCC_APB2ENR
ldr r1, [r0]
orr r1, 1 << 4 // IOPCEN
str r1, [r0]

// configure PC13 as open-drain output with 10 MHz speed
ldr r0, =0x40011000 + 0x04 // GPIOC_CRH
ldr r1, [r0]
bic r1, 1 << 20 // MODE13:0
orr r1, 1 << 21 // MODE13:1
orr r1, 1 << 22 // CNF13:0
bic r1, 1 << 23 // CNF13:1
str r1, [r0]

blink_loop:

// wait loop
ldr r0, =1000000
delay_loop:
subs r0, 1
bne delay_loop

// toggle PC13
ldr r0, =0x40011000 + 0x0c // GPIOC_ODR
ldr r1, [r0]
eor r1, 1 << 13 // ODR13
str r1, [r0]

b blink_loop
