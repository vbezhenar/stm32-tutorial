#include <stdint.h>

static const uint32_t rcc_base_address = 0x40021000;
static const uint32_t rcc_apb2enr_address = rcc_base_address + 0x18;
static const uint32_t gpioc_base_address = 0x40011000;
static const uint32_t gpioc_crh_address = gpioc_base_address + 0x04;
static const uint32_t gpioc_odr_address = gpioc_base_address + 0x0c;

static void enable_port_clock(void);
static void configure_pin(void);
static void toggle_pin(void);

void reset_exception_handler(void)
{
    enable_port_clock();
    configure_pin();

    // blink loop
    for (;;)
    {
        // wait loop
        for (volatile uint32_t delay = 100000; delay > 0; delay--)
        {
        }

        toggle_pin();
    }
}

static volatile uint32_t *rcc_apb2enr_pointer = (uint32_t *)rcc_apb2enr_address;

static void enable_port_clock(void)
{
    // enable I/O port C clock
    uint32_t rcc_apb2enr_value = *rcc_apb2enr_pointer;
    rcc_apb2enr_value |= 0x00000010;
    *rcc_apb2enr_pointer = rcc_apb2enr_value;
}

static volatile uint32_t *gpioc_crh_pointer = (uint32_t *)gpioc_crh_address;

static void configure_pin(void)
{
    // configure PC13 as open-drain output with 10 MHz speed
    uint32_t gpioc_crh_value = *gpioc_crh_pointer;
    gpioc_crh_value |= 0x00600000;
    gpioc_crh_value &= ~0x00900000;
    *gpioc_crh_pointer = gpioc_crh_value;
}

static volatile uint32_t *gpioc_odr_pointer = (uint32_t *)gpioc_odr_address;
static uint32_t gpioc_odr_value;

static void toggle_pin(void)
{
    // toggle PC13
    gpioc_odr_value = *gpioc_odr_pointer;
    gpioc_odr_value ^= 0x00002000;
    *gpioc_odr_pointer = gpioc_odr_value;
}
