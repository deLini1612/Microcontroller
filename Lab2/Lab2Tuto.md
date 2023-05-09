Lab 02: Blink LED base on off-on-off sequence signal from switch
==============

***Tutorial for lab2 in course microcontroller at HUST***

**Author:** *deLini1612*

---

**In a nutshell:**
1. Write function to set, clear the value of 1 bit (using MASK)
2. Write a program which detects *off-on-of* sequence signal from switch then blinked LED in 1 second

---
# Table of Contents


---
# Requirements

1. Read ***esp32-c3_technical_reference_manual*** and determine addresses of GPIO 1 and GPIO 2
2. Write function to set, clear the value of 1 bit (using MASK)
3. Write a program which detects *off-on-of* sequence signal from switch then turn on LED for 1 second
4. Prepare physical kit (connect switch to GPIO 1 and LED to GPIO 2)

---

# Task 1: Determine addresses of GPIO 1 and GPIO 2 and addresses of related registers
1. Navigate to page 90, "*table 3-3. Module/Peripheral Address Mapping*", you can see ***base address*** of GPIO is 0x6000_4000
2. Jump into page 167-168, you can see offset address of register with base address is ***0x6000_4000 (GPIO)***:
    - `GPIO_FUNC1_OUT_SEL_CFG_REG`: Configuration register for GPIO1 output - offset `0x0558`
    - `GPIO_FUNC2_OUT_SEL_CFG_REG`: Configuration register for GPIO2 output - offset `0x055C`
    - `GPIO_IN_REG`: GPIO input register - offset 0x003C (better set and clear using `GPIO_IN_W1TS_REG`, `GPIO_IN_W1TC_REG`)
    - `GPIO_OUT_REG`: GPIO output register - offset 0x0004 (better set and clear using `GPIO_OUT_W1TS_REG`, `GPIO_OUT_W1TC_REG`)
    - `GPIO_ENABLE_REG`: GPIO output enable register - offset 0x0020
3. Jump into page 168-169, you can see offset address of register with base address is ***0x6000_9000 (IO MUX)***:
    - `IO_MUX_GPIO1_REG`: IO MUX configuration register for pin XTAL_32K_N (GPIO1) - offset `0x0008`
    - `IO_MUX_GPIO2_REG`: IO MUX configuration register for pin GPIO2 - offset `0x000C`


---
# Task 2: Write function to set, clear the value of 1 bit (using MASK)
- A ***MASK*** of n-th bit is a binary number which is has only n-th bit equals 1, all the remaining bits are 0. It means that a mask of n bit is "1 shift left n bit"
    ```C
    #define MASK(n) ((uint32_t) 1U << (n))
    ```
- If you want to clear n-th bit, you have to ***AND with invert*** of MASK(n)
- If you want to set n-th bit, you have to ***OR*** with MASK(n)
- You can find example functions to set, clear a bit we built in file [example_set_clear.c](./example_set_clear.c)

---
# Task 3: Blink LED base on off-on-off sequence signal from switch

## SPEC
- Input: 1 bit represent state of switch (0 = OFF and 1 = ON)
- Output: 1 bit represent state of LED (0 = OFF and 1 = ON)
- Relationship between input and output: System detects sequence OFF-ON-OFF (0-1-0) at the input pin, then set output to blink LED for 1 second

---
## Architecture
- 2 functions
  - event(): detects 0-1-0 sequence at input pin
  - main(): calls event() function, then set, clear output to blink LED

---
## Algorithm for event() function
Using 2 type of loop:
  - P: detect positive edge of input (0->1)
  - N: detect negative edge of input (1->0)

> Using 3 loop in N-P-N order to detect (use the first additional N loop in order to verify that in case at the start time, if switch is ON: only when ON-OFF-ON-OFF the LED will blink instead of blink right after ON-OFF)

---
## Source code

---
# 6. Result
