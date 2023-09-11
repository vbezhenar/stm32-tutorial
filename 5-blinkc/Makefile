blinkc.bin: blinkc.elf
	arm-none-eabi-objcopy -O binary -j main blinkc.elf blinkc.bin

flash: blinkc.bin
	st-flash --reset --connect-under-reset write blinkc.bin 0x08000000

blinkc.elf: linker.ld blinkc.o
	arm-none-eabi-ld -T linker.ld -o blinkc.elf blinkc.o

blinkc.o: blinkc.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -O0 -c -o blinkc.o blinkc.c

clean:
	rm -f blinkc.o blinkc.elf blinkc.bin
