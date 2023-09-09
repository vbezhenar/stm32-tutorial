loop.bin: loop.elf
	arm-none-eabi-objcopy -O binary -j main loop.elf loop.bin

flash: loop.bin
	st-flash write loop.bin 0x08000000

loop.elf: linker.ld loop.o
	arm-none-eabi-ld -T linker.ld -o loop.elf loop.o

loop.o: loop.s
	arm-none-eabi-as -o loop.o loop.s

clean:
	rm -f loop.o loop.elf loop.bin
