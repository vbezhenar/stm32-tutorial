blinkc.bin: blinkc.elf
	arm-none-eabi-objcopy -O binary blinkc.elf blinkc.bin

flash: blinkc.bin
	st-flash write blinkc.bin 0x08000000

blinkc.elf: linker.ld reset_exception_handler.o blinkc.o
	arm-none-eabi-ld -T linker.ld -o blinkc.elf reset_exception_handler.o blinkc.o

reset_exception_handler.o: reset_exception_handler.s
	arm-none-eabi-as -o reset_exception_handler.o reset_exception_handler.s

blinkc.o: blinkc.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -O0 -c -o blinkc.o blinkc.c

clean:
	rm -f blinkc.bin blinkc.elf startup.o blinkc.o
