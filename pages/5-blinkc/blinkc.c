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
