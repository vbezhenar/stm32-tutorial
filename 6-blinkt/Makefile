blinkt.bin: blinkt.elf
	arm-none-eabi-objcopy -O binary blinkt.elf blinkt.bin

flash: blinkt.bin
	st-flash write blinkt.bin 0x08000000

blinkt.elf: linker.ld reset_exception_handler.o blinkt.o
	arm-none-eabi-ld -T linker.ld -o blinkt.elf reset_exception_handler.o blinkt.o

reset_exception_handler.o: reset_exception_handler.s
	arm-none-eabi-as -o reset_exception_handler.o reset_exception_handler.s

blinkt.o: blinkt.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -O3 -c -o blinkt.o blinkt.c

clean:
	rm -f blinkt.bin blinkt.elf startup.o blinkt.o
