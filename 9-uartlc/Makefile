CPPFLAGS := -I$(CMSIS_STM32F1)/Device/ST/STM32F1xx/Include
CPPFLAGS += -I$(CMSIS_STM32F1)/Include
CPPFLAGS += -DSTM32F103xB 
CFLAGS := -mcpu=cortex-m3
CFLAGS += -g -std=c17 -Wall -Wextra -Wpedantic
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Os
LDSCRIPT := STM32F103XB_FLASH.ld
LDFLAGS := -Wl,-T,$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections -Wl,--no-warn-rwx-segments

AS := arm-none-eabi-as
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

uartlc.elf: startup_stm32f103xb.o system_stm32f1xx.o main.o os.o
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

uartlc.bin: uartlc.elf
	$(OBJCOPY) -O binary $<  $@

startup_stm32f103xb.o: $(CMSIS_STM32F1)/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s
	$(AS) -o $@ $<

system_stm32f1xx.o: $(CMSIS_STM32F1)/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

flash: uartlc.bin
	st-flash --connect-under-reset write $< 0x08000000

clean:
	rm -f uartlc.elf uartlc.bin *.o
