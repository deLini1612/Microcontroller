/*
Lib for ESP32C3 IO MUX Register Address
See page 168 Technical Reference for details
*/

#ifndef _IO_MUX_REG_H_
#define _IO_MUX_REG_H_
#include "regBase.h"

// Configuration Registers
#define IO_MUX_PIN_CTRL_REG       (C3_IO_MUX + 0x0)                //R/W
#define IO_MUX_GPIO_REG_BASE      (C3_IO_MUX + 0x4)
#define IO_MUX_GPIO_REG(pin)      (IO_MUX_GPIO_REG_BASE + 4*pin)   //WT

// Version Register
#define IO_MUX_DATE_REG           (C3_IO_MUX + 0xFC)    //R/W  

#endif