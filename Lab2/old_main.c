#include <mdk.h>

static int led_pin = 2;
static int led_state = 0;
static int sw_state = 0;
static int sw_pin = 1;

// event function to detect a off-on-off event of sw
int event()
{
    sw_state = gpio_read(sw_pin);
    while (sw_state)
    {
        delay_ms(10);
        sw_state = gpio_read(sw_pin);
    }
    printf("\n SWITCH: %s", (sw_state)?"ON":"OFF");
    while (!sw_state)
    {
        delay_ms(10);
        sw_state = gpio_read(sw_pin);
    }
    printf("\n SWITCH: %s", (sw_state)?"ON":"OFF");
    while (sw_state)
    {
        delay_ms(10);
        sw_state = gpio_read(sw_pin);
    }
    printf("\n SWITCH: %s", (sw_state)?"ON":"OFF");
    return 1;
}

int main(void)
{
    gpio_output(led_pin);
    gpio_input(sw_pin);
    wdt_disable();
    // sw_state = gpio_read(sw_pin);

    for (;;)
    {
        gpio_write(led_pin, led_state); // Blink an LED
        led_state = event();
        gpio_write(led_pin, led_state);
        printf("\n Blink LED");
        led_state = !led_state;
        delay_ms(500);
    }

    return 0;
}
