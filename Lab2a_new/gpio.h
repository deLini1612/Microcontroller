// API GPIO
#pragma once
#include <stdbool.h>
#include "gpioReg.h"
#include "ioMuxReg.h"
#include "bitfield.h"

// define GPIO input type
#define PULL_UP 1
#define PULL_DOWN 0

// define gpio isr type
#define RISING_EDGE 1
#define FALLING_EDGE 2
#define ANY_EDGE 3 
#define LOW_LEVEL 4
#define HIGH_LEVEL 5 

static inline void gpio_output_enable(int pin)
{
  SET_BIT(GPIO_ENABLE_W1TS_REG, pin);
}

static inline void gpio_output_disable(int pin)
{
  CLEAR_BIT(GPIO_ENABLE_W1TS_REG, pin);
}

static inline void gpio_output(int pin)
{
  SET_BIT(GPIO_FUNC_OUT_SEL_CFG_REG(pin), 9); // Force output enable is sourced from GPIO_ENABLE_REG
  WRITE_FIELD(GPIO_FUNC_OUT_SEL_CFG_REG(pin), 0, 7, 128); // GPIO_OUT_REG and GPIO_ENABLE_REG are selected as output value and output enable.
  gpio_output_enable(pin);
  printf("gpio_output successfully... ");
}

static inline void gpio_write(int pin, bool value)
{
  if (value)
    SET_BIT(GPIO_OUT_REG, pin);
  else
    CLEAR_BIT(GPIO_OUT_REG, pin);
}

static inline void gpio_toggle(int pin)
{
  TOGGLE_BIT(GPIO_OUT_REG, pin);
}

static inline void gpio_input(int pin, int type)
{
  gpio_output_disable(pin);         // Disable output
  SET_BIT(IO_MUX_GPIO_REG(pin), 9); // GPIO input enable
  if (type == PULL_UP)
    SET_BIT(IO_MUX_GPIO_REG(pin), 8); // Enable pull-up
  else
    SET_BIT(IO_MUX_GPIO_REG(pin), 7); // Enable pull-down
  
  printf("gpio_input successfully... ");
}

static inline bool gpio_read(int pin)
{
  return GET_BIT(GPIO_IN_REG, pin);
}

static inline void gpio_isr_enable(int pin)
{
  SET_BIT(GPIO_PIN_REG(pin), INT_ENA);
  printf("gpio_isr_enable successfully... ");
}

static inline void gpio_isr_type(int pin, int type)
{
  WRITE_FIELD(GPIO_PIN_REG(pin), INT_TYPE_LSB, INT_TYPE_MSB, type);
  printf("gpio_isr_type successfully... ");
}

static inline void clear_gpio_intr(int pin)
{
  SET_BIT(GPIO_STATUS_W1TC_REG, pin);
}