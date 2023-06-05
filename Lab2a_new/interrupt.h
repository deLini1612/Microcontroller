#pragma once
#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdio.h>
#include "interruptReg.h"
#include "csr(esp_idf).h"
#include "bitfield.h"

// ISR_TYPE
#define EDGE_ISR 1
#define LEVEL_ISR 0

// Set up vector table
void setup_isr_vector(uint32_t mtvec_val)
{
    // uint32_t mtvec_val = (uint32_t)&_vector_table;
    mtvec_val |= 1; // Set MODE field to treat MTVEC as a vector base address
    RV_WRITE_CSR(mtvec, mtvec_val);
    printf("setup_isr_vector successfully... ");
}

// Function to get the value MIE and clear MIE
bool get_and_clear_mie ()
{
    // get old value of mstatus then clear MIE bit
    uint32_t old_mstatus = RV_CLEAR_CSR(mstatus, MSTATUS_MIE);
    bool old_mie = ((old_mstatus & MSTATUS_MIE) == 0)?0:1;
    printf("done get_and_clear_mie() with old MIE is %d... ", old_mie);
    return old_mie;
}

// Clear MIE bit to disable interrupt globally
void isr_global_disable()
{
    RV_CLEAR_CSR(mstatus, MSTATUS_MIE);
    printf("isr_global_disable successfully... ");
}

// Set MIE bit to enable
void isr_global_enable()
{
    RV_SET_CSR(mstatus, MSTATUS_MIE);
    printf("isr_global_enable successfully... ");
}

void isr_map(int source_address, int isr_number)
{
    WRITE_FIELD(source_address, SOURCE_MAP_LSB, SOURCE_MAP_MSB, isr_number);
    printf("isr_map successfully... ");
}

void set_isr_type(int isr_number, int type)
{
    if (type == EDGE_ISR)
        SET_BIT(INTERRUPT_CORE0_CPU_INT_TYPE_REG, isr_number);
    else
        CLEAR_BIT(INTERRUPT_CORE0_CPU_INT_TYPE_REG, isr_number);
}

void set_isr_pri(int isr_number, int pri)
{
    WRITE_FIELD(INTERRUPT_CORE0_CPU_INT_PRI_REG(isr_number), PRI_MAP_LSB, PRI_MAP_MSB, pri);
}

void isr_enable(int isr_number)
{
    asm volatile("fence");
    SET_BIT(INTERRUPT_CORE0_CPU_INT_ENABLE_REG, isr_number);
}

void isr_setup(int isr_number, int type, int pri)
{
    set_isr_type(isr_number, type);
    set_isr_pri(isr_number, pri);
    isr_enable(isr_number);
    printf("isr_setup successfully... ");
}

// Trap handler
typedef void (*isr_func_pointer)();
static isr_func_pointer isr_handlers[31];
void setup_handler(int isr_number, void (*handler)())
{
    isr_handlers[isr_number] = handler;
    printf("setup_handler successfully... ");
}

void handle_trap()
{
    printf("Helu from handle_trap");
    uint32_t isr_cause = RV_READ_CSR(mcause);
    uint32_t isr_status0 = *REG(INTERRUPT_CORE0_INTR_STATUS_0_REG);
    uint32_t isr_status1 = *REG(INTERRUPT_CORE0_INTR_STATUS_1_REG);
    printf("Jump to trap handler with mcause = %d...., status0 = %x, status1=%x", isr_cause, isr_status0, isr_status1);
    if (isr_cause == 9)
        isr_handlers[(int)isr_cause]();
}
