#include <stdio.h>
#include "interrupt.h"
#include "gpio.h"
#include "mdk_remain.h"
#include "userTrapHandler.h"

static int led_pin = 3;
static int button_pin = 9;
static int interrupt_num = 9;


extern void _vector_table();

int main()
{
    //Set up gpio
    gpio_output(led_pin);
    gpio_input(button_pin, PULL_UP);
    wdt_disable();


    // Set up ISR
    isr_global_disable();
    setup_isr_vector((uint32_t)&_vector_table);
    isr_setup(interrupt_num, LEVEL_ISR, 5);
    isr_global_enable();
    
    // Set up GPIO interrupt
    isr_map(INTERRUPT_CORE0_GPIO_INTERRUPT_PRO_MAP_REG, interrupt_num);
    setup_handler(interrupt_num, gpio_isr_handler);
    gpio_isr_type(button_pin, LOW_LEVEL);
    clear_gpio_intr(button_pin);
    gpio_isr_enable(button_pin);



    printf("\nmcause = %x\n", RV_READ_CSR(mcause));
    printf("mtvec = %x\n", RV_READ_CSR(mtvec));
    //Check _vector_table is 40380400
    //True: 40380401
    
    printf("gpio_enable_reg (output) = %x\n", *REG(GPIO_ENABLE_REG));
    //True: 0x8 (1000), pin 3 set as output
    
    printf("gpio_func_out_sel_cfg (output) = %x\n", *REG(GPIO_FUNC_OUT_SEL_CFG_REG(led_pin)));
    //True: 280 (10 1000 0000)

    printf("io_mux_gpio_reg (input) = %x\n", *REG(IO_MUX_GPIO_REG(button_pin)));
    //True: x11 0000 0000

    printf("gpio_interrupt_pro_map = %x\n", *REG(INTERRUPT_CORE0_GPIO_INTERRUPT_PRO_MAP_REG));
    //True: 9

    while(1) {
        printf("\nFrom main loop: button status is: %x\n", gpio_read(button_pin));
        printf("mcause = %x - status0 = %x - status1 = %x \n", RV_READ_CSR(mcause), *REG(INTERRUPT_CORE0_INTR_STATUS_0_REG), *REG(INTERRUPT_CORE0_INTR_STATUS_1_REG));
        //why status 0 = 8 (source = 3) when havent press button, then turn to 10008 (source = 3 and 16) when press the button

        printf("gpio_status_reg = %x\n", *REG(GPIO_STATUS_REG));
        //True: has interrupt: 0x200 (10 0000 0000); else will be 0
        
        printf("gpio_pin_reg = %x\n", *REG(GPIO_PIN_REG(button_pin)));
        //True: 2200 (10 0010 0000 0000) for low_level + itr_ena

        printf("cpu_int_enable = %x\n", *REG(INTERRUPT_CORE0_CPU_INT_ENABLE_REG));
        //True: 1<<9 = 0x200 

        printf("cpu_int_type = %x\n", *REG(INTERRUPT_CORE0_CPU_INT_TYPE_REG));
        //True: 0x200 (if edge), 0 (if level)
        
        printf("cpu_int_eip_status = %x\n", *REG(INTERRUPT_CORE0_CPU_INT_EIP_STATUS_REG));
        //True: 0 if not press, 200 if press

        printf("cpu_int_pri_9 = %x\n", *REG(INTERRUPT_CORE0_CPU_INT_PRI_REG(interrupt_num)));  //True: = priority
        //True: 5
        
        printf("cpu_int_thresh = %x\n", *REG(INTERRUPT_CORE0_CPU_INT_THRESH_REG));    //Default = 1
        
        printf("mstatus = %x\n", RV_READ_CSR(mstatus));
        //89 = 1000 1001 = MIE enable, MPIE = enable

        delay_ms(1000);
    }

    return 0;
}