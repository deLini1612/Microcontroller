#include "gpio.h"
#include "mdk_remain.h"

void gpio_isr_handler()
{
    printf("Helu from gpio_isr_handler....");
    if (gpio_read(9)==1)
    {
        printf("Button is pressed");
        gpio_write(3,1);
        delay_ms(1000);
        gpio_write(3,0);
    }
}