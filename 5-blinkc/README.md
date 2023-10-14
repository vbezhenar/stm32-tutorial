# Часть 5: Мигаем на C

Эта часть будет комбинацией частей 3 и 4. Мы перепишем код из части 3 на C,
используя "инфраструктуру" для сборки из части 4 и познакомимся с некоторыми не
всегда очевидными моментами, которые надо помнить при работе с микроконтроллером
из кода на C.

Скрипт линкера и файл `reset_exception_handler.s` не поменяются, приведём лишь
код программы на C:

`blinkc.c`:

```c
#include <stdint.h>

static void enable_port_clock(void);
static void configure_pin(void);
static void toggle_pin(void);

void start(void)
{
    enable_port_clock();
    configure_pin();

    // blink loop
    for (;;)
    {
        // wait loop
        for (uint32_t delay = 1000000; delay > 0; delay--)
        {
        }

        toggle_pin();
    }
}

static void enable_port_clock(void)
{
    // enable I/O port C clock
    uint32_t rcc_base_address = 0x40021000;
    uint32_t rcc_apb2enr_address = rcc_base_address + 0x18;
    uint32_t rcc_apb2enr_iopcen = 1 << 4;

    uint32_t *rcc_apb2enr_pointer = (uint32_t *)rcc_apb2enr_address;
    uint32_t rcc_apb2enr_value = *rcc_apb2enr_pointer;
    rcc_apb2enr_value |= rcc_apb2enr_iopcen;
    *rcc_apb2enr_pointer = rcc_apb2enr_value;
}

static void configure_pin(void)
{
    // configure PC13 as open-drain output with 2 MHz speed
    uint32_t gpioc_base_address = 0x40011000;
    uint32_t gpioc_crh_address = gpioc_base_address + 0x04;
    uint32_t gpiox_crh_mode13_0 = 1 << 20;
    uint32_t gpiox_crh_mode13_1 = 1 << 21;
    uint32_t gpiox_crh_cnf13_0 = 1 << 22;
    uint32_t gpiox_crh_cnf13_1 = 1 << 23;

    uint32_t *gpioc_crh_pointer = (uint32_t *)gpioc_crh_address;
    uint32_t gpioc_crh_value = *gpioc_crh_pointer;
    gpioc_crh_value &= ~gpiox_crh_mode13_0;
    gpioc_crh_value |= gpiox_crh_mode13_1;
    gpioc_crh_value |= gpiox_crh_cnf13_0;
    gpioc_crh_value &= ~gpiox_crh_cnf13_1;
    *gpioc_crh_pointer = gpioc_crh_value;
}

static void toggle_pin(void)
{
    // toggle PC13
    uint32_t gpioc_base_address = 0x40011000;
    uint32_t gpioc_odr_address = gpioc_base_address + 0x0c;
    uint32_t gpiox_odr_odr13 = 1 << 13;

    uint32_t *gpioc_odr_pointer = (uint32_t *)gpioc_odr_address;
    uint32_t gpioc_odr_value = *gpioc_odr_pointer;
    gpioc_odr_value ^= gpiox_odr_odr13;
    *gpioc_odr_pointer = gpioc_odr_value;
}
```

Очень важный нюанс: нужно обратить внимание, чтобы эта программа компилировалась
без оптимизации, с флагом `-O0`. Если всё сделано верно, то `make clean flash`
прошьёт программу на микроконтроллер, и он замигает светодиодом.

Если же эту программу скомпилировать с флагом `-O3` (не забудьте выполнить
`make clean`), то произойдёт странное событие. После запуска микроконтроллер
включит светодиод, да так и оставит его включённым. Чтобы разобраться, в чём
дело, попробуем дизассемблировать то, что нам скомпилировал gcc:

```

blinkc.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <start>:
   0:	4906      	ldr	r1, [pc, #24]	@ (1c <start+0x1c>)
   2:	4807      	ldr	r0, [pc, #28]	@ (20 <start+0x20>)
   4:	684b      	ldr	r3, [r1, #4]
   6:	6982      	ldr	r2, [r0, #24]
   8:	f423 0310 	bic.w	r3, r3, #9437184	@ 0x900000
   c:	f042 0210 	orr.w	r2, r2, #16
  10:	f443 03c0 	orr.w	r3, r3, #6291456	@ 0x600000
  14:	6182      	str	r2, [r0, #24]
  16:	604b      	str	r3, [r1, #4]
  18:	e7fe      	b.n	18 <start+0x18>
  1a:	bf00      	nop
  1c:	40011000 	.word	0x40011000
  20:	40021000 	.word	0x40021000
```

Впечатляющая работа компилятора, 57 строк кода на C он скомпилировал всего в 10
машинных инструкций. Приведём аналогичный псевдокод:

```
r1 := 0x40011000;
r0 := 0x40021000;
r3 := memory[r1 + 4];
r2 := memory[r0 + 24];
r1[20] := 0;
r1[23] := 0;
r2[5] := 1;
r3[21] := 1;
r3[22] := 1;
memory[r0 + 24] := r2;
memory[r1 + 4] := r3;
loop: goto loop
```

Видно, что имеется код для инициализации порта C и настройки вывода PC13, однако
наш цикл мигания куда-то пропал. Оказывается компилятор решил, что наш цикл
абсолютно бесполезный и если его убрать, то программа будет работать гораздо
быстрей. Не будем вдаваться в дебри стандарта C и неопределённого поведения,
просто скажем, что на всех переменных, которые работают с памятью, нужно ставить
`volatile`. В этом случае компилятор будет знать, что мы не просто так байты
туда-сюда пересылаем, а делаем важную работу и не будет удалять наш код.

Исправленный код:

```c
#include <stdint.h>

static void enable_port_clock(void);
static void configure_pin(void);
static void toggle_pin(void);

void start(void)
{
    enable_port_clock();
    configure_pin();

    // blink loop
    for (;;)
    {
        // wait loop
        for (volatile uint32_t delay = 1000000; delay > 0; delay--)
        {
        }

        toggle_pin();
    }
}

static void enable_port_clock(void)
{
    // enable I/O port C clock
    uint32_t rcc_base_address = 0x40021000;
    uint32_t rcc_apb2enr_address = rcc_base_address + 0x18;
    uint32_t rcc_apb2enr_iopcen = 1 << 4;

    volatile uint32_t *rcc_apb2enr_pointer = (uint32_t *)rcc_apb2enr_address;
    uint32_t rcc_apb2enr_value = *rcc_apb2enr_pointer;
    rcc_apb2enr_value |= rcc_apb2enr_iopcen;
    *rcc_apb2enr_pointer = rcc_apb2enr_value;
}

static void configure_pin(void)
{
    // configure PC13 as open-drain output with 10 MHz speed
    uint32_t gpioc_base_address = 0x40011000;
    uint32_t gpioc_crh_address = gpioc_base_address + 0x04;
    uint32_t gpiox_crh_mode13_0 = 1 << 20;
    uint32_t gpiox_crh_mode13_1 = 1 << 21;
    uint32_t gpiox_crh_cnf13_0 = 1 << 22;
    uint32_t gpiox_crh_cnf13_1 = 1 << 23;

    volatile uint32_t *gpioc_crh_pointer = (uint32_t *)gpioc_crh_address;
    uint32_t gpioc_crh_value = *gpioc_crh_pointer;
    gpioc_crh_value &= ~gpiox_crh_mode13_0;
    gpioc_crh_value |= gpiox_crh_mode13_1;
    gpioc_crh_value |= gpiox_crh_cnf13_0;
    gpioc_crh_value &= ~gpiox_crh_cnf13_1;
    *gpioc_crh_pointer = gpioc_crh_value;
}

static void toggle_pin(void)
{
    // toggle PC13
    uint32_t gpioc_base_address = 0x40011000;
    uint32_t gpioc_odr_address = gpioc_base_address + 0x0c;
    uint32_t gpiox_odr_odr13 = 1 << 13;

    volatile uint32_t *gpioc_odr_pointer = (uint32_t *)gpioc_odr_address;
    uint32_t gpioc_odr_value = *gpioc_odr_pointer;
    gpioc_odr_value ^= gpiox_odr_odr13;
    *gpioc_odr_pointer = gpioc_odr_value;
}
```

Этот код работает при любой оптимизации.

Помните, что компилятор C ничего не знает о том, что при записи чего-то в память
вы работаете с периферийным устройством. Для него эти операции выглядят, как
обычная работа с оперативной памятью и без дополнительных усилий он может
оптимизировать и выбрасывать ваши инструкции, если посчитает, что это приведёт к
более быстрому коду. В спорных случаях не стесняйтесь проверять скомпилированный
код дизассемблером.

Полный код доступен на
[гитхабе](https://github.com/vbezhenar/stm32-tutorial/blob/main/5-blinkc).
