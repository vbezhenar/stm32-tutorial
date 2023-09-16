# Часть 7. Hello world через UART

UART это протокол, по которому микроконтроллер может взаимодействовать с
компьютером. Его можно использовать для печати отладочных сообщений, или для
полноценного взаимодействия с программой, запущенной на компьютере. USART это
что-то вроде расширенной версия UART, которую мы использовать не будем, в
документации обычно используется именно этот термин.

В datasheet в разделе 2.1 можно найти, что на нашем микроконтроллере установлено
три USART устройства. В разделе 3 таблице 5 можно найти, какие выводы для какого
устройства можно использовать. В нашем примере будем использовать USART3, т.к.
его выводы TX и RX можно назначить на выводы микроконтроллера 21 и 22
соответственно, которые на плате разведены недалеко от вывода GND. Вывод 21
иначе называтся PB10, вывод 22 иначе называется PB11.

USART-устройство отвечает только за "логику" обмена данными. Для того, чтобы на
проводах появились или считались сигналы, используется GPIO порт, который мы уже
использовали ранее, мигая светодиодом.

Если ваша плата не оснащена "гребёнкой", придётся немного поработать паяльником.
Для подключения к компьютеру нужен USB-UART переходник. К нему нужно подключить
3 провода: GND, TX и RX. При этом TX переходника подключается к RX платы, а RX
переходника подключается к TX платы.

Примерный план действий будет следующим:

1. Активировать и настроить USART3, а также GPIO выводы PB10 и PB11.
2. Отправить байты, соответствующие строке `"Hello world 1\n"`. Последняя цифра
   будет меняться от 0 до 9.
3. Подождать примерно секунду и перейти на п.2.

Для активации GPIO выводов нужно включить тактирование порта B, похожую задачу
мы уже решали в предыдущих частях. Для этого нужно в регистр `RCC_APB2ENR`
записать единицу в 3 бит `IOPBEN`.

Вывод PB10 соответствует выводу TX, т.е. он будет использован для отправки
данных. Его нужно настроить в режиме alternate function output Push-pull 2MHz.
Для этого нужно записать соответствующие биты в регистр `GPIOB_CRH`: `MODE10` =
`0b10`, `CNF10` = `0b10`. Вывод PB11 соответствует выводу RX и используется для
приёма данных. В нашей программе приёма данных не будет, поэтому его мы
настраивать не будем.

Для активации USART3 нужно включить тактирование соответствующего устройства с
помощью RCC. В Reference Manual, разделе 3.1, изображении 1 можно увидеть, что
USART3 подключен к шине APB1. В разделе 7.3.8 описан регистр `RCC_APB1ENR`, в
котором 18 бит `USART3EN` отвечает за включение USART3, в который нужно будет
записать единицу.

В разделе 3.3 в таблице 3 можно найти адреса, выделенные для взаимодействия с
USART3, это `0x4000_4800` - `0x4000_4BFF`. Весь раздел 27 посвящен USART, в
разделе 27.6 описаны все регистры, используемые при работе с USART.

Обязательно нужно настроить скорость передачи, для наших целей подойдёт 9600 бод
(примерно 9600 битов в секунду или 1200 байтов в секунду). В разделе 27.3.4
приведено подробное описание того, как настраивать скорость. Она зависит от
частоты тактирования PCLK1, подробней про частоты тактирования и их настройки
можно почитать в разделе 7. По умолчанию на нашей плате эта частота равна 8 МГц.
Для частоты передачи в 9600 бод значение USARTDIV должно быть равно 8000000 /
9600 / 16 = 52 + 1/12. На дробную часть выделено 4 бита, поэтому дробная часть
может принимать значения 0, 1/16, ..., 15/16. Ближе всего к желаемому значению
число 52 + 1/16. Для этого числа DIV_Mantissa = 52 = 0x34, DIV_Fraction = 1.
Значением регистра USART_BRR будет 0x0341.

Для включения USART нужно в регистре `USART_CR1` записать единицу в 13 бит `UE`,
а также записать единицу в порт `TE`, чтобы включить передатчик.

Остальные настройки менять не будем, значения по умолчанию нам подойдут.

Итоговый код для включения USART3:

```
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
    gpiob_crh_value &= ~gpiox_crh_cnf10_0;
    gpiob_crh_value |= gpiox_crh_cnf10_1;
    *gpiob_crh_pointer = gpiob_crh_value;

    // enable USART3 clock
    volatile uint32_t *rcc_apb1enr_pointer = (uint32_t *)rcc_apb1enr_address;
    uint32_t rcc_apb1enr_value = *rcc_apb1enr_pointer;
    rcc_apb1enr_value |= rcc_apb1enr_usart3en;
    *rcc_apb1enr_pointer = rcc_apb1enr_value;

    // set baud rate 9600
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
```

После такой инициализации устройство USART3 готово к отправке данных.

Чтобы отправить байт, нужно записать его в регистр `USART_DR`.В регистре
`USART_SR` имеется бит `TXE`. Перед записью данных в регистр `USART_DR` бит
`TXE` равен единице. После записи данных в регистр `USART_DR`, бит `TXE` будет
равен нулю, пока данные передаются из регистра `USART_DR` в т.н. transmit shift
register, откуда USART уже начнёт передавать его по проводам, бит за битом. Если
нужно отправить следующий байт, необходимо дождаться, пока бит `TXE` не станет
равен единице. Это можно сделать пустым циклом. После этого можно записать
следующий байт и так далее.

Можно обратить внимание, что в нашем микроконтроллере имеется лишь один регистр
`USART_DR`. На самом деле за ним скрываются два регистра: `USART_TDR` и
`USART_RDR`, которые используются для передачи и приёма данных соответственно.
При записи данных в регистр `USART_DR` они попадают в регистр `USART_TDR`, а при
чтении из регистра `USART_DR` данные считываются из регистра `USART_RDR`. На
других микроконтроллерах эти регистры могут быть разделены.

Для отправки даных будем использовать следующий код:

```c
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
```

Конечная программа будет выглядеть примерно так:

```c
static void send_hello(void)
{
    static char hello[16] = "Hello world 0\r\n";
    static uint8_t counter = 0;

    counter = (counter + 1) % 10;
    hello[13] = counter + '0';
    send_string(hello);
}

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
```

Если всё было сделано верно, то после её запуска начнётся передача данных от
платы к UART-USB-переходнику, подключенному к компьютеру. На моём переходнике
при этом помигивает красный светодиод в момент активности. Для того, чтобы
вывести эти данные в терминал, в линуксе можно использовать команду `screen`:

```
screen /dev/ttyUSB0
```

Кроме screen можно использовать программу minicom, ну и многие другие.

Имя устройства можно увидеть в `dmesg` сразу после подключения переходника к
компьютеру. Возможно потребуется настроить udev или запускать команду `screen`
от рута. Для выхода из screen нажмите Ctrl+A, затем K, затем Y.

Интереса ради давайте подсчитаем, сколько циклов тратит микроконтроллер на то,
чтобы послать один символ.

Скорость передачи 9600 бод, т.е. 9600 бит в секунду. Частота работы процессора -
8 МГц. Итого процессор тратит более 800 циклов, ожидая, пока запишется очередной
бит, более 6.6 тысяч циклов, пока запишется очередной байт и около 100 тыс.
циклов на всю фразу из 15 байтов. Это, конечно, не слишком оптимальное
использование и без того ограниченных ресурсов. На практике лучше использовать
прерывания: можно настроить USART так, чтобы процессор получал прерывание в тот
момент, когда USART готов передать очередной байт, а всё остальное время он
сможет заниматься чем-либо ещё.

Полный код доступен на
[гитхабе](https://github.com/vbezhenar/stm32-tutorial/blob/main/7-uart).
