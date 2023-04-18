	.file	"boot.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.type	soc_init, @function
soc_init:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	addi	s0,sp,16
	li	a5,1611399168
	addi	a5,a5,8
	lw	a4,0(a5)
	li	a5,1611399168
	addi	a5,a5,8
	andi	a4,a4,-4
	sw	a4,0(a5)
	li	a5,1611399168
	addi	a5,a5,8
	lw	a4,0(a5)
	li	a5,1611399168
	addi	a5,a5,8
	ori	a4,a4,5
	sw	a4,0(a5)
	li	a5,1611399168
	addi	a5,a5,88
	li	a4,688128
	addi	a4,a4,1024
	sw	a4,0(a5)
	li	a5,1073741824
	addi	a5,a5,1416
	li	a0,160
	jalr	a5
	nop
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	soc_init, .-soc_init
	.local	s_heap_start
	.comm	s_heap_start,4,4
	.local	s_heap_end
	.comm	s_heap_end,4,4
	.local	s_brk
	.comm	s_brk,4,4
	.align	1
	.globl	sbrk
	.type	sbrk, @function
sbrk:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	lui	a5,%hi(s_brk)
	lw	a5,%lo(s_brk)(a5)
	sw	a5,-20(s0)
	lui	a5,%hi(s_brk)
	lw	a4,%lo(s_brk)(a5)
	lw	a5,-36(s0)
	add	a4,a4,a5
	lui	a5,%hi(s_heap_end)
	lw	a5,%lo(s_heap_end)(a5)
	bleu	a4,a5,.L3
	li	a5,0
	j	.L4
.L3:
	lui	a5,%hi(s_brk)
	lw	a4,%lo(s_brk)(a5)
	lw	a5,-36(s0)
	add	a4,a4,a5
	lui	a5,%hi(s_brk)
	sw	a4,%lo(s_brk)(a5)
	lw	a5,-20(s0)
.L4:
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	sbrk, .-sbrk
	.align	1
	.globl	_reset
	.type	_reset, @function
_reset:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	lui	a5,%hi(s_brk)
	lui	a4,%hi(_end)
	addi	a4,a4,%lo(_end)
	sw	a4,%lo(s_brk)(a5)
	lui	a5,%hi(s_brk)
	lw	a4,%lo(s_brk)(a5)
	lui	a5,%hi(s_heap_start)
	sw	a4,%lo(s_heap_start)(a5)
	lui	a5,%hi(s_heap_end)
	lui	a4,%hi(_eram)
	addi	a4,a4,%lo(_eram)
	sw	a4,%lo(s_heap_end)(a5)
	lui	a5,%hi(_sbss)
	addi	a5,a5,%lo(_sbss)
	sw	a5,-20(s0)
	j	.L6
.L7:
	lw	a5,-20(s0)
	addi	a4,a5,1
	sw	a4,-20(s0)
	sb	zero,0(a5)
.L6:
	lw	a4,-20(s0)
	lui	a5,%hi(_ebss)
	addi	a5,a5,%lo(_ebss)
	bltu	a4,a5,.L7
	call	soc_init
	call	main
.L8:
	j	.L8
	.size	_reset, .-_reset
	.ident	"GCC: (crosstool-NG esp-2022r1) 11.2.0"
