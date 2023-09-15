usart.bin: usart.elf
	arm-none-eabi-objcopy -O binary usart.elf usart.bin

flash: usart.bin
	st-flash write usart.bin 0x08000000

usart.elf: linker.ld reset_exception_handler.o usart.o
	arm-none-eabi-ld -T linker.ld -o usart.elf reset_exception_handler.o usart.o

reset_exception_handler.o: reset_exception_handler.s
	arm-none-eabi-as -o reset_exception_handler.o reset_exception_handler.s

usart.o: usart.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -g -O0 -c -o usart.o usart.c

clean:
	rm -f usart.bin usart.elf startup.o usart.o
