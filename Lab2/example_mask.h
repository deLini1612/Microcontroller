#pragma once

#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdio.h>

#define GPIO_BASE 0x60004000
#define GPIO_OUT_FUNC 341
#define REG(x) ((volatile uint32_t *) (x))
#define MASK(n) ((uint32_t)1U << (n))
#define pin 5

static inline void init_gpio_output()
{
    REG(GPIO_BASE)[GPIO_OUT_FUNC + pin] = MASK(9) | 128;
    REG(GPIO_BASE)[0x0024] |= MASK(pin);
}

// set or clear gpio value
static inline void gpio_write(int value)
{
    if (value)
        REG(GPIO_BASE)[2] |= MASK(pin);
    else
        REG(GPIO_BASE)[3] |= MASK(pin);
}