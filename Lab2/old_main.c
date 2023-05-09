#include <mdk.h>

static int led_pin = 2;
static int led_state = 0;

// event function to detect a off-on-off event of sw

int main(void)
{
    gpio_output(led_pin);
    wdt_disable();

    for (;;)
    {
        printf("LED: %d\n", led_state);
        gpio_write(led_pin, led_state); // Blink an LED
        led_state = !led_state;
        delay_ms(500);
    }

    return 0;
}
