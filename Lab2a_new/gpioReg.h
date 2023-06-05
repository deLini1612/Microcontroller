/*
Lib for ESP32C3 GPIO Register Address
See page 167 Technical Reference for details
*/

#ifndef _GPIO_REG_H_
#define _GPIO_REG_H_
#include "regBase.h"

// Configuration Registers
#define GPIO_BT_SELECT_REG        (C3_GPIO + 0x0)   //R/W
#define GPIO_OUT_REG              (C3_GPIO + 0x4)   //R/W/SS
#define GPIO_OUT_W1TS_REG         (C3_GPIO + 0x8)   //WT
#define GPIO_OUT_W1TC_REG         (C3_GPIO + 0xC)   //WT
#define GPIO_ENABLE_REG           (C3_GPIO + 0x20)  //R/W/SS
#define GPIO_ENABLE_W1TS_REG      (C3_GPIO + 0x24)  //WT
#define GPIO_ENABLE_W1TC_REG      (C3_GPIO + 0x28)  //WT
#define GPIO_STRAP_REG            (C3_GPIO + 0x38)  //RO
#define GPIO_IN_REG               (C3_GPIO + 0x3C)  //RO
#define GPIO_STATUS_REG           (C3_GPIO + 0x44)  //R/W/SS
#define GPIO_STATUS_W1TS_REG      (C3_GPIO + 0x48)  //WT
#define GPIO_STATUS_W1TC_REG      (C3_GPIO + 0x4C)  //WT
#define GPIO_PCPU_INT_REG         (C3_GPIO + 0x5C)  //RO
#define GPIO_PCPU_NMI_INT_REG     (C3_GPIO + 0x60)  //RO
#define GPIO_STATUS_NEXT_REG      (C3_GPIO + 0x14C) //RO

// Pin Configuration Registers
#define GPIO_PIN_REG_BASE         (C3_GPIO + 0x74)
#define GPIO_PIN_REG(pin)         (GPIO_PIN_REG_BASE + pin*4)  //R/W
// Bit field in GPIO_PIN_REG 
#define SYNC2_BYPASS_LSB            0    
#define SYNC2_BYPASS_MSB            1    
#define PAD_DRIVER                  2        
#define SYNC1_BYPASS_LSB            3
#define SYNC1_BYPASS_MSB            4   
#define INT_TYPE_LSB                7
#define INT_TYPE_MSB                9  
#define WAKEUP_ENABLE               10      
#define CONFIG_LSB                  11
#define CONFIG_MSB                  12  
#define INT_ENA                     13     

// Input Function Configuration Registers
#define GPIO_FUNC_IN_SEL_CFG_REG_BASE   (C3_GPIO + 0x154)
#define GPIO_FUNC_IN_SEL_CFG_REG(n)     (GPIO_FUNC_IN_SEL_CFG_REG_BASE + n*4)  //R/W

// Output Function Configuration Registers
#define GPIO_FUNC_OUT_SEL_CFG_REG_BASE   (C3_GPIO + 0x554)
#define GPIO_FUNC_OUT_SEL_CFG_REG(n)     (GPIO_FUNC_OUT_SEL_CFG_REG_BASE + n*4)  //R/W

// Version Register
#define GPIO_DATE_REG    (C3_GPIO + 0x6FC)  //R/W

// Clock Gate Register
#define GPIO_CLOCK_GATE_REG (C3_GPIO + 0x62C) //R/W


// SDM Register page 169
// Configuration registers
#define GPIOSD_SIGMADELTA0_REG      (C3_GPIO + 0x0F00 + 0x0)   //R/W
#define GPIOSD_SIGMADELTA1_REG      (C3_GPIO + 0x0F00 0x4)     //R/W
#define GPIOSD_SIGMADELTA2_REG      (C3_GPIO + 0x0F00 + 0x8)   //R/W
#define GPIOSD_SIGMADELTA3_REG      (C3_GPIO + 0x0F00 + 0xC)   //R/W
#define GPIOSD_SIGMADELTA_CG_REG    (C3_GPIO + 0x0F00 + 0x20)  //R/W
#define GPIOSD_SIGMADELTA_MISC_REG  (C3_GPIO + 0x0F00 + 0x24)  //R/W

// Version register
#define GPIOSD_SIGMADELTA_VERSION_REG (C3_GPIO + 0x0F00 + 0x28) //R/W

#endif