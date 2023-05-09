#pragma once

#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdio.h>

#define GPIO_BASE 0x60004000
#define IO_MUX_BASE 0x60009000
#define MASK(n) ((uint32_t)1U << (n))
#define SET(x, n) ((x) |= MASK(n))

typedef struct
{
    uint32_t bit_select;
    uint32_t out;
    uint32_t out_w1ts;
    uint32_t out_w1tc; // 0x000C
    uint32_t dummy[4]; // 10, 14, 18, 1C
    uint32_t enable;   // 0x0020
    uint32_t enable_w1ts;
    uint32_t enable_w1tc; // 0x0028
    uint32_t dummy1[3];   // 2c, 30, 34
    uint32_t strap;       // 0x0038
    uint32_t in;          // 0x003c
    uint32_t dummy2;      // 40
    uint32_t status;      // 0x0044
    uint32_t status_w1ts;
    uint32_t status_w1tc;  // 0x004c
    uint32_t dummy3[3];    // 50, 54, 58
    uint32_t pcpu_int;     // 0x005c
    uint32_t pcpu_nmi_int; // 0x0060
    uint32_t dummy4[4];    // 64, 68, 6c, 70
    uint32_t pin_n[22];    // 0x0074 to 0x00c8
    uint32_t dummy5[32];
    uint32_t status_next;          // 0x014c
    uint32_t dummy6;               // 150
    uint32_t func_in_sel_cfg[128]; // 0x0154 to 0x0350
    uint32_t dummy7[128];
    uint32_t func_out_sel_cfg[22]; // 0x0554 to 0x05a8
    uint32_t dummy8[32];
    uint32_t clock_gate; // 0x062c
    uint32_t dummy9[51];
    uint32_t date; // 0x06fc
} gpio_matrix;

typedef struct
{
    uint32_t pin_ctrl;
    uint32_t gpio_reg[22]; // 0x0004 to 0x0058
    uint32_t dummy[40];    // 0x005c to 0x00f8
    uint32_t date_reg;     // 0x00fc
} io_mux;

volatile gpio_matrix *gpio = (volatile gpio_matrix *)(GPIO_BASE);
volatile io_mux *io = (volatile io_mux *)(IO_MUX_BASE);

// Set a gpio pin as simple output
static inline void init_gpio_output(int pin)
{
    // Write 128 to gpio_func_out_sel field to set up simple output
    SET(gpio->func_out_sel_cfg[pin], 7);
    // Set gpio_func_oen_sel to force the output enable to be source from gpio_enable_reg
    SET(gpio->func_out_sel_cfg[pin], 9);
    SET(gpio->enable_w1ts, pin); // enable gpio output
}

// set or clear gpio value
static inline void gpio_write(int pin, int value)
{
    if (value)
        SET(gpio->out_w1ts, pin);
    else
        SET(gpio->out_w1tc, pin);
}

// Set a gpio pin as simple input
static inline void init_gpio_input(int pin)
{
    SET(gpio->enable_w1tc, pin); // disable gpio output
    SET(io->gpio_reg[pin], 7);   // enable internal pull-down
    SET(io->gpio_reg[pin], 9);   // enable input of pin
    SET(io->gpio_reg[pin], 15);  // filter enabled to remove glitch
}

static bool gpio_read(int pin)
{
    // return value of pin-th bit of gpio_in_reg
    return gpio->in & MASK(pin) ? 1 : 0;
}

static inline void wdt_disable(void)
{
    // disable RTC watchdog write protection
    *((volatile uint32_t *)(0x600080a8)) = 0x50d83aa1;
    *((volatile uint32_t *)(0x60008090)) = 0; // Disable RTC WDT
    *((volatile uint32_t *)(0x6000808c)) = 0;

    // bootloader_super_wdt_auto_feed()
    *((volatile uint32_t *)(0x600080b0)) = 0x8F1D312A;
    SET(*((volatile uint32_t *)(0x600080ac)), 31);
    *((volatile uint32_t *)(0x600080b4)) = 0;

    *((volatile uint32_t *)(0x6001f0fc)) &= ~MASK(9); // TIMG_REGCLK -> disable TIMG_WDT_CLK
    *((volatile uint32_t *)(0x6001f048)) = 0;         // Disable TG0 WDT
    *((volatile uint32_t *)(0x60020048)) = 0;         // Disable TG1 WDT
}

static inline void spin(volatile unsigned long count)
{
    while (count--)
        asm volatile("nop");
}

static inline uint64_t systick(void)
{
    *((volatile uint32_t *)(0x60023004)) = MASK(30); // TRM 10.5
    spin(1);
    return ((uint64_t) * ((volatile uint32_t *)(0x60023040)) << 32) | *((volatile uint32_t *)(0x60023044));
}

static inline uint64_t uptime_us(void)
{
    return systick() >> 4;
}

static inline void delay_ms(unsigned long ms)
{
    uint64_t until = uptime_us() + ms * 1000;
    while (uptime_us() < until)
        spin(1);
}