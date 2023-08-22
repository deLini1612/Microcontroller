Microcontroller
===========
This repo consists of labs exercises from Microcontrller Course 20222 at HUST and related document. In this course, we will use:

- **MCU:** ESP32-C3
- **ISA:** RISC-V
- **Kit:** ESP32-C3-32S C3FN4
- **Frame work**: Baremetal (Lab1, Lab1.5), esp-idf (Lab 2, Lab 3, Lab 4)

## List of Labs
1. Lab1
   - Install toolchain
   - Blink LED using given example baremetal code
2. Lab1.5
   - Write function to set, clear the value of 1 bit (using MASK)
   - Write a baremetal program which detects *off-on-of* sequence signal from switch then blinked LED in 1 secondDetect an ON-O blink LED
3. Lab 2
   - Using esp-idf framework to build a programe:
     - Using timer interrupt to print "" every 1 second
     - Blink LED with period 1 second in GPIO8 using timer interrupt
4. Lab 3
   - Write a progame based on esp-idf framework to implement a digital clock (have hour, minute, second)
   - Print the current time every second to the moniter
5. Lab 4
   - Using esp-idf to write a programe to transmit digital clock value (Digital clock is built in lab 3 and update time by using timer interrupt). But in this lab, transmit data using UART instead of `printf` function
   - Using UART controller of ESP32C3 and a USB to COM devices to display time value in PuTTY Terminal
   - Tx: GPIO18; Rx: GPIO19
6. Final Term Lab
   - Build your own function to implement a soft UART Tx and Rx phase (NOT ALLOW using UART function of esp-idf but can use timmer and GPIO function)
   - Tx: GPIO18; Rx: GPIO19
   - Using the function you just build to implement a simple digital clock programe:
     - Init a digital clock when open programe
     - Using a set of AT command to communicate between PC and ESP32C3 kit through UART protocol:
       - If we enter command `AT+R`: PC asked for current time value. Then ESP32C3 transmit the current time data through Tx pin of ESP to PC
       - If we enter command `AT+W hh:mm:ss`: Set time for digital clock. ESP will set the coresponding time for clock if the time user wanna set is valid and respond by transmiting new time to PuTTY console. Otherwise, ESP will respond "Invalid time"
       - If enter a not supported command, ESP will respond "Invalid command"
       - Enter `quit` to exit the programe