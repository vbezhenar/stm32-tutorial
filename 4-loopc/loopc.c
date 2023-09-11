#include <stdint.h>

static const uint32_t loop_start = 0x12345678;
static const uint32_t loop_increment = 3;
static uint32_t loop_value_1 = loop_start;
static uint32_t loop_value_2;

void start(void)
{
    for (;;)
    {
        loop_value_2 = loop_value_1 + loop_increment;
        loop_value_1 = loop_value_2;
    }
}
