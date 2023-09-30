loopc.bin: loopc.elf
	arm-none-eabi-objcopy -O binary loopc.elf loopc.bin

flash: loopc.bin
	st-flash --connect-under-reset write loopc.bin 0x08000000

loopc.elf: linker.ld reset_exception_handler.o loopc.o
	arm-none-eabi-ld -T linker.ld -o loopc.elf reset_exception_handler.o loopc.o

reset_exception_handler.o: reset_exception_handler.s
	arm-none-eabi-as -o reset_exception_handler.o reset_exception_handler.s

loopc.o: loopc.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -O0 -c -o loopc.o loopc.c

clean:
	rm -f loopc.bin loopc.elf startup.o loopc.o
