#include <stddef.h>
#include <stdint.h>

static void enable_usart(void);

static void send_hello(void);

void start(void)
{
    enable_usart();
    for (;;)
    {
        send_hello();
        for (volatile uint32_t counter = 0; counter < 1000000; counter++)
        {
        }
    }
}

static void enable_usart(void)
{
    uint32_t rcc_base_address = 0x40021000;
    uint32_t rcc_apb2enr_address = rcc_base_address + 0x18;
    uint32_t rcc_apb2enr_iopben = 1 << 3;
    uint32_t rcc_apb1enr_address = rcc_base_address + 0x1c;
    uint32_t rcc_apb1enr_usart3en = 1 << 18;
    uint32_t gpiob_base_address = 0x40010c00;
    uint32_t gpiob_crh_address = gpiob_base_address + 0x04;
    uint32_t gpiox_crh_mode10_0 = 1 << 8;
    uint32_t gpiox_crh_mode10_1 = 1 << 9;
    uint32_t gpiox_crh_cnf10_0 = 1 << 10;
    uint32_t gpiox_crh_cnf10_1 = 1 << 11;
    uint32_t usart3_base_address = 0x40004800;
    uint32_t usart3_brr_address = usart3_base_address + 0x08;
    uint32_t usart3_cr1_address = usart3_base_address + 0x0c;
    uint32_t usartx_cr1_ue = 1 << 13;
    uint32_t usartx_cr1_te = 1 << 3;

    // enable port B clock
    volatile uint32_t *rcc_apb2enr_pointer = (uint32_t *)rcc_apb2enr_address;
    uint32_t rcc_apb2enr_value = *rcc_apb2enr_pointer;
    rcc_apb2enr_value |= rcc_apb2enr_iopben;
    *rcc_apb2enr_pointer = rcc_apb2enr_value;

    // configure PB10 as alternate function output push-pull 2MHz
    volatile uint32_t *gpiob_crh_pointer = (uint32_t *)gpiob_crh_address;
    uint32_t gpiob_crh_value = *gpiob_crh_pointer;
    gpiob_crh_value &= ~gpiox_crh_mode10_0;
    gpiob_crh_value |= gpiox_crh_mode10_1;
    gpiob_crh_value |= gpiox_crh_cnf10_0;
    gpiob_crh_value |= gpiox_crh_cnf10_1;
    *gpiob_crh_pointer = gpiob_crh_value;

    // enable USART3 clock
    volatile uint32_t *rcc_apb1enr_pointer = (uint32_t *)rcc_apb1enr_address;
    uint32_t rcc_apb1enr_value = *rcc_apb1enr_pointer;
    rcc_apb1enr_value |= rcc_apb1enr_usart3en;
    *rcc_apb1enr_pointer = rcc_apb1enr_value;

    // set baud rate
    volatile uint32_t *usart3_brr_pointer = (uint32_t *)usart3_brr_address;
    uint32_t usart3_brr_value = 0x0341;
    *usart3_brr_pointer = usart3_brr_value;

    // enable USART3 and transmitter
    volatile uint32_t *usart3_cr1_pointer = (uint32_t *)usart3_cr1_address;
    uint32_t usart3_cr1_value = *usart3_cr1_pointer;
    usart3_cr1_value |= usartx_cr1_ue;
    usart3_cr1_value |= usartx_cr1_te;
    *usart3_cr1_pointer = usart3_cr1_value;
}

static void send_string(const char *string);

static void send_hello(void)
{
    static char hello[16] = "Hello world 0\r\n";
    static uint8_t counter = 0;

    counter = (counter + 1) % 10;
    hello[12] = counter + '0';
    send_string(hello);
}

static void send_string(const char *string)
{
    uint32_t usart3_base_address = 0x40004800;
    uint32_t usart3_sr_address = usart3_base_address + 0x00;
    uint32_t usart3_sr_txe = 1 << 7;
    uint32_t usart3_dr_address = usart3_base_address + 0x04;

    volatile uint32_t *usart3_sr_pointer = (uint32_t *)usart3_sr_address;
    volatile uint32_t *usart3_dr_pointer = (uint32_t *)usart3_dr_address;

    uint32_t usart3_sr_value;

    for (size_t index = 0; string[index] != '\0'; index++)
    {
        char ch = string[index];

        // wait until data is transferred to the shift register
        do
        {
            usart3_sr_value = *usart3_sr_pointer;
        } while ((usart3_sr_value & usart3_sr_txe) == 0);

        // write data to the data register
        *usart3_dr_pointer = (uint32_t)ch & 0x000000ff;
    }
}
