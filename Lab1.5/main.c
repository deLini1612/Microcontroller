#include "lib.h"

static int led_pin = 3;
static int led_state = 0;
static int sw_state = 0;
static int sw_pin = 9;

int event()
{
  sw_state = gpio_read(sw_pin);
  while (sw_state)
  {
    delay_ms(1);
    sw_state = gpio_read(sw_pin);
  }
  printf("\n SWITCH: %s", (sw_state) ? "ON" : "OFF");
  while (!sw_state)
  {
    delay_ms(1);
    sw_state = gpio_read(sw_pin);
  }
  printf("\n SWITCH: %s", (sw_state) ? "ON" : "OFF");
  while (sw_state)
  {
    delay_ms(1);
    sw_state = gpio_read(sw_pin);
  }
  printf("\n SWITCH: %s", (sw_state) ? "ON" : "OFF");
  return 1;
}

int main(void)
{
  init_gpio_output(led_pin);
  init_gpio_input(sw_pin);
  wdt_disable();
  // sw_state = gpio_read(sw_pin);

  for (;;)
  {
    gpio_write(led_pin, led_state); // Blink an LED
    led_state = event();
    gpio_write(led_pin, led_state);
    printf("\n Blink LED");
    led_state = !led_state;
    delay_ms(1000);
  }

  return 0;
}