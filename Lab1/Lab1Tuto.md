Lab 01: Blinky
==============

***Tutorial for lab1 in course microcontroller at HUST***

**Author:** *deLini1612*

---

**In a nutshell:**
1. Download espressif for esp32c3, check for the toolchain link then modify the toolchain link in [```build.mk```](../mdk/esp32c3/build.mk) to the toolchain link you just finded. Change target architecture in [makefile (examples)](../mdk/examples/Makefile) to esp32c3.
2. Modify the [```main.c```](../mdk/examples/blinky/main.c) file then run make in [blinky dir](../mdk/examples/blinky/)
3. Run make in [esputil dir](../mdk/esputil/) then flash into ESP32C3

---
# Table of Contents
- [Lab 01: Blinky](#lab-01-blinky)
- [Table of Contents](#table-of-contents)
- [Requirements](#requirements)
  - [Software](#software)
  - [Hardware](#hardware)
- [Objective](#objective)
- [Step by step tutorial](#step-by-step-tutorial)
  - [Step 1: Find toolchain path](#step-1-find-toolchain-path)
  - [Step 2: Set PATH environment variable](#step-2-set-path-environment-variable)
  - [Step 3: Build firmware](#step-3-build-firmware)
  - [Step 4: Flash code into MCU](#step-4-flash-code-into-mcu)
  - [Step 5: Change LED pin back to GPIO and build test board LED](#step-5-change-led-pin-back-to-gpio-and-build-test-board-led)
- [Result](#result)
  - [Toolchain path](#toolchain-path)
  - [Blinky using GPIO and a LED](#blinky-using-gpio-and-a-led)

---
# Requirements

## Software
- Download Standard Toolchain Espressif for ESP32-C3 stable and follow the tutorial [here](https://docs.espressif.com/projects/esp-idf/en/v5.0.1/esp32c3/get-started/linux-macos-setup.html)
- Clone 2 repos [ESP32 flashing utility](https://github.com/cpq/esputil) and [ESP32/ESP32C3 SDK](https://github.com/cpq/mdk)

## Hardware
- ESP32-C3 kit
- Cable to load code

---
# Objective
- Download toolchain, find link of toolchain.
- Build, load (flash) example blinky code (given in [here](../mdk/examples/blinky)) then make a simple LED circuit and run code to blink LED.

---
# Step by step tutorial

## Step 1: Find toolchain path
There are **2 ways** in order to find toolchain path:
1. Using find **command** in ubuntu: 
    ```
    find ~/ -type f -name "riscv32-esp-elf-gcc"
    find ~/ -type f -name "riscv32-esp-elf-g++"
    find ~/ -type f -name "riscv32-esp-elf-ld"
    ```
2. **Build an example project** (Eg: blink in ```/esp/esp-idf/examples/get-started/blink/```) then look up the log file in build dir
    - Build blink project: Navigate to your project dir ->  config your project (set target chip, set up project variables) -> build project
        ```
        get_idf
        cd ~/esp/esp-idf/examples/get-started/blink/
        idf.py set-target esp32c3
        idf.py menuconfig
        idf.py build
        ```
    - Find toolchain path by looking up log file

---
## Step 2: Set PATH environment variable
To use toolchain, you will need to update your ```PATH``` environment variable.
1. Add ```PATH``` environment variable directly to your ```~/.bashrc``` then run command ```source ~/.bashrc```:
    ```
    export PATH=$PATH:/home/nplink/.espressif/tools/riscv32-esp-elf/esp-2022r1-11.2.0/riscv32-esp-elf/bin/
    ```
2. Run those command:
    ```
    export MDK=/home/nplink/Documents/VXL/mdk    # Points to MDK directory
    export ARCH=esp32c3         # Valid choices: esp32 esp32c3
    ```

---
## Step 3: Build firmware
1. Modify [makefile (examples)](../mdk/examples/Makefile) first line: set ARCHITECTURES to esp32c3
2. Go to our project directory: [blinky dir](../mdk/examples/blinky/) and check the MakeFile -> Makefile include ```/home/nplink/Documents/VXL/mdk/esp32c3/build.mk```, so we need to check [```build.mk```](../mdk/esp32c3/build.mk)
3. Modify [```build.mk```](../mdk/esp32c3/build.mk): set TOOLCHAIN to **riscv32-esp-elf**
4. Check [```main.c```](../mdk/examples/blinky/main.c) file in project dir then modify `led_pin` to `3` to blink the LED in NodeMCU ESP32-C3 (instead of GPIO) for testing purpose. Disable Watch Dog Timer (wdt) by add `wdt_disable();` line **before for loop** (Why? we will discus later)
5. Run `make` in [blinky dir](../mdk/examples/blinky/). After run `make`, run `ls` to check if it done right (you will see **2 new file** *firmware.bin* and *firmware.elf*)

---
## Step 4: Flash code into MCU
To flash code to MCU, we will need to use **esputil**, you can find more information [here](https://github.com/cpq/esputil) or follow my work:

1. Navigate to [esputil dir](../mdk/esputil) then check if there is a executable file esputil (if not, run `make`)
2. Flash code into MCU: Navigate to [blinky dir](../mdk/examples/blinky/). You need to give port rwxrwxrwx permissions then using `make flash` to flash code into MCU, using `make clean` and `make flash` to re flash new code
```
sudo chmod 777 /dev/ttyUSB0
make flash
```

---
## Step 5: Change LED pin back to GPIO and build test board LED
1. In [```main.c```](../mdk/examples/blinky/main.c) file in project dir modify `led_pin` to `LED1` (LED1 is define in **mdk.h** and it's pin 2).
2. Check **Pin function definition table** in ESP32-C3-32S specification, pin 2 is IO2 GPIO, set up a test board with LED (anode is connected with IO2 and cathode is connected with GND)
3. Run command `make monitor` to see status of LED1
    > Notice that when pin 2 (IO2) is 0V (Low level), the LED is off 

---
# Result

## Toolchain path
Our toolchain path:
```
1. /home/nplink/.espressif/tools/riscv32-esp-elf/esp-2022r1-11.2.0/riscv32-esp-elf/bin/riscv32-esp-elf-gcc

2. /home/nplink/.espressif/tools/riscv32-esp-elf/esp-2022r1-11.2.0/riscv32-esp-elf/bin/riscv32-esp-elf-g++

3. /home/nplink/.espressif/tools/riscv32-esp-elf/esp-2022r1-11.2.0/riscv32-esp-elf/bin/riscv32-esp-elf-ld
```
<p align="center">
  <img alt="Using find command" src="./Pics/find_toolchain.png" width="82%">
</p>
<p style="text-align: center;">Using find command</p>

<p align="center">
  <img alt="By build a sample project" src="./Pics/toolchain_path.png" width="82%">
</p>
<p style="text-align: center;">Finding by build a sample project</p>

---
## Blinky using GPIO and a LED
1. Modify ***led_pin*** static int variable to ***3*** then run `make flash` (result can be seen in video uploaded in [Result dir](./Result))
2. Set up experiment: We use a test board, a 10k Ohm resistor and a red LED to set up our experiment. Resistor is connect series with LED, anode of LED is connected with IO2 and cathode is connected with GND (like below pic)
3. Run `make clean`, `make flash` and `make monitor` to clean, load code and show status of GPIO LED pin in terminal (result can be seen in video uploaded in [Result dir](./Result))
<p align="center">
  <img alt="make flash and make monitor" src="./Pics/flash_pin2.png" width="82%">
</p>
<p style="text-align: center;">Make flash and make monitor</p>