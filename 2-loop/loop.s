.cpu cortex-m3
.syntax unified
.thumb

.global reset_exception_handler

.section code

reset_exception_handler:
add r0, r0, 1
bl reset_exception_handler
