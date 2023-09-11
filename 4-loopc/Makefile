loopc.bin: loopc.elf
	arm-none-eabi-objcopy -O binary loopc.elf loopc.bin

flash: loopc.bin
	st-flash write loopc.bin 0x08000000

loopc.elf: loopc.ld startup.o loopc.o
	arm-none-eabi-ld -T loopc.ld -o loopc.elf startup.o loopc.o

startup.o: startup.s
	arm-none-eabi-as -o startup.o startup.s

loopc.o: loopc.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -O0 -c -o loopc.o loopc.c

clean:
	rm -f loopc.bin loopc.elf startup.o loopc.o
