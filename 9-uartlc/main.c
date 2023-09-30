#include <stdio.h>
#include "stm32f103xb.h"

static void enable_usart(void);

int main(void)
{
    enable_usart();
    for (int counter = 0;; counter++)
    {
        printf("Hello, world %d\r\n", counter);
        for (volatile int unused = 0; unused < 1000000; unused++)
        {
        }
    }
}

static void enable_usart(void)
{
    // enable port B clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // configure PB10 as alternate function output push-pull 2MHz
    uint32_t gpiob_crh = GPIOB->CRH;
    gpiob_crh &= ~GPIO_CRH_MODE10_0;
    gpiob_crh |= GPIO_CRH_MODE10_1;
    gpiob_crh &= ~GPIO_CRH_CNF10_0;
    gpiob_crh |= GPIO_CRH_CNF10_1;
    GPIOB->CRH = gpiob_crh;

    // enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // set baud rate
    USART3->BRR = 0x0341;

    // enable USART3 and transmitter
    uint32_t usart3_cr1 = USART3->CR1;
    usart3_cr1 |= USART_CR1_UE;
    usart3_cr1 |= USART_CR1_TE;
    USART3->CR1 = usart3_cr1;
}
