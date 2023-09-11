loop.bin: loop.elf
	arm-none-eabi-objcopy -O binary -j flash loop.elf loop.bin

flash: loop.bin
	st-flash write loop.bin 0x08000000

loop.elf: loop.ld loop.o
	arm-none-eabi-ld -T loop.ld -o loop.elf loop.o

loop.o: loop.s
	arm-none-eabi-as -o loop.o loop.s

clean:
	rm -f loop.o loop.elf loop.bin
