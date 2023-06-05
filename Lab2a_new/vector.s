    /* This is the vector table. MTVEC points here.
     *
     * Use 4-byte intructions here. 1 instruction = 1 entry of the table.
     * The CPU jumps to MTVEC (i.e. the first entry) in case of an exception,
     * and (MTVEC & 0xfffffffc) + (mcause & 0x7fffffff) * 4, in case of an interrupt.
     */
    .balign 0x100
    .global _vector_table
    .type _vector_table, @function
_vector_table:
    j _panic_handler            /* exception handler, entry 0 */
    .rept 31
    j _interrupt_handler        /* 31 identical entries, all pointing to the interrupt handler */
    .endr
    .size _vector_table, .-_vector_table

    /* This should be the exception handler.
     * It doesn't do anything useful yet, but you can set a breakpoint here, to see if an exception happens.
     */
    .global _panic_handler
    .type _panic_handler, @function
_panic_handler:
    j _panic_handler
    .size  _panic_handler, .-_panic_handler

    /* This is the interrupt handler.
     * It is declared as a weak function, and can be overridden.
     */
    .global _interrupt_handler
    .weak _interrupt_handler
    .type _interrupt_handler, @function
_interrupt_handler: 
  
    addi sp, sp, -72
    sw x1,  0(sp)
    sw x2,  4(sp)
    sw x3,  8(sp)
    sw x5,  12(sp)
    sw x6,  16(sp)
    sw x7,  20(sp)
    sw x10, 24(sp)
    sw x11, 28(sp)
    sw x12, 32(sp)
    sw x13, 36(sp)
    sw x14, 40(sp)
    sw x15, 44(sp)
    sw x16, 48(sp)
    sw x17, 52(sp)
    sw x28, 56(sp)
    sw x29, 60(sp)
    sw x30, 64(sp)
    sw x31, 68(sp)

    call handle_trap
    
    # restore regs
    lw x1,  0(sp)
    lw x2,  4(sp)
    lw x3,  8(sp)
    lw x5,  12(sp)
    lw x6,  16(sp)
    lw x7,  20(sp)
    lw x10, 24(sp)
    lw x11, 28(sp)
    lw x12, 32(sp)
    lw x13, 36(sp)
    lw x14, 40(sp)
    lw x15, 44(sp)
    lw x16, 48(sp)
    lw x17, 52(sp)
    lw x28, 56(sp)
    lw x29, 60(sp)
    lw x30, 64(sp)
    lw x31, 68(sp)
    addi sp, sp, 72

    MRET 
    .size  _interrupt_handler, .-_interrupt_handler
