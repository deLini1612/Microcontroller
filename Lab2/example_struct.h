#pragma once

#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdio.h>

#define GPIO_BASE 0x60004000
#define IO_MUX_BASE 0x60009000
#define pin 5

typedef union 
{
    struct {
        unsigned pin0:1;
        unsigned pin1:1;
        unsigned pin2:1;
        unsigned pin3:1;
        unsigned pin4:1;
        unsigned pin5:1;
        unsigned pin6:1;
        unsigned pin7:1;
        unsigned pin8:1;
        unsigned pin9:1;
        unsigned pin10:1;
        unsigned pin11:1;
        unsigned pin12:1;
        unsigned pin13:1;
        unsigned pin14:1;
        unsigned pin15:1;
        unsigned pin16:1;
        unsigned pin17:1;
        unsigned pin18:1;
        unsigned pin19:1;
        unsigned pin20:1;
        unsigned pin21:1;
        unsigned invalid: 4;
        unsigned reserved: 6;
    };
    uint32_t reg;
} write_1;

typedef union 
{
    struct {
        unsigned out_sel:8;
        unsigned out_inv_sel:1;
        unsigned oen_sel:1;
        unsigned oen_inv_sel:1;
        unsigned reserved:21;
    };
    uint32_t reg;
} func_out_sel_cfg_format;

typedef struct
{
    uint32_t bit_select;
    uint32_t out;
    write_1 out_w1ts;
    write_1 out_w1tc; // 0x000C
    uint32_t dummy[4]; // 10, 14, 18, 1C
    uint32_t enable;   // 0x0020
    write_1 enable_w1ts;
    write_1 enable_w1tc; // 0x0028
    uint32_t dummy1[7];
    write_1 status_w1ts;
    write_1 status_w1tc;  // 0x004c
    uint32_t dummy3[321];
    func_out_sel_cfg_format func_out_sel_cfg[22]; // 0x0554 to 0x05a8
    uint32_t dummy8[85];
} gpio_matrix;

volatile gpio_matrix *gpio = (volatile gpio_matrix *)(GPIO_BASE);

// Set a gpio pin as simple output
static inline void init_gpio_output()
{
    // Write 128 to gpio_func_out_sel field to set up simple output
    gpio->func_out_sel_cfg[pin].out_sel = 128;
    // Set gpio_func_oen_sel to force the output enable to be source from gpio_enable_reg
    gpio->func_out_sel_cfg[pin].oen_sel =1;
    gpio->enable_w1ts.pin5 = 1;
}

// set or clear gpio value
static inline void gpio_write( int value)
{
    if (value) gpio->out_w1ts.pin5 = 1;
    else gpio->out_w1tc.pin5 = 1;
}