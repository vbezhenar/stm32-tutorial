CPPFLAGS := -I$(CMSIS_STM32F1)/Device/ST/STM32F1xx/Include
CPPFLAGS += -I$(CMSIS_STM32F1)/Include
CPPFLAGS += -DSTM32F103xB 
CFLAGS := -mcpu=cortex-m3 -g -std=c17 -Wall -Wextra -Wpedantic
CFLAGS += -Os
LDSCRIPT := STM32F103XB_FLASH.ld

AS := arm-none-eabi-as
CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy

uarts.elf: $(LDSCRIPT) startup_stm32f103xb.o system_stm32f1xx.o main.o
	$(LD) -o $@ -T $^

uarts.bin: uarts.elf
	$(OBJCOPY) -O binary $<  $@

startup_stm32f103xb.o: $(CMSIS_STM32F1)/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s
	$(AS) -o $@ $<

system_stm32f1xx.o: $(CMSIS_STM32F1)/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

flash: uarts.bin
	st-flash --connect-under-reset write $< 0x08000000

clean:
	rm -f uarts.elf uarts.bin *.o
