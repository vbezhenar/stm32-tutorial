blink.bin: blink.elf
	arm-none-eabi-objcopy -O binary -j flash blink.elf blink.bin

flash: blink.bin
	st-flash --connect-under-reset write blink.bin 0x08000000

blink.elf: linker.ld blink.o
	arm-none-eabi-ld -T linker.ld -o blink.elf blink.o

blink.o: blink.s
	arm-none-eabi-as -o blink.o blink.s

clean:
	rm -f blink.o blink.elf blink.bin
