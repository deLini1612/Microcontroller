	.file	"example_struct.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	gpio
	.section	.sdata,"aw"
	.align	2
	.type	gpio, @object
	.size	gpio, 4
gpio:
	.word	1610629120
	.text
	.align	1
	.type	gpio_output, @function
gpio_output:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	lui	a5,%hi(gpio)
	lw	a5,%lo(gpio)(a5)
	li	a4,-128
	sb	a4,1368(a5)
	lui	a5,%hi(gpio)
	lw	a5,%lo(gpio)(a5)
	lw	a4,1368(a5)
	ori	a4,a4,512
	sw	a4,1368(a5)
	lui	a5,%hi(gpio)
	lw	a5,%lo(gpio)(a5)
	lw	a4,36(a5)
	ori	a4,a4,2
	sw	a4,36(a5)
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	gpio_output, .-gpio_output
	.align	1
	.type	gpio_write, @function
gpio_write:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lw	a5,-20(s0)
	beq	a5,zero,.L3
	lui	a5,%hi(gpio)
	lw	a5,%lo(gpio)(a5)
	lw	a4,8(a5)
	ori	a4,a4,2
	sw	a4,8(a5)
	j	.L5
.L3:
	lui	a5,%hi(gpio)
	lw	a5,%lo(gpio)(a5)
	lw	a4,12(a5)
	ori	a4,a4,2
	sw	a4,12(a5)
.L5:
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	gpio_write, .-gpio_write
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	addi	s0,sp,16
	call	gpio_output
	li	a0,1
	call	gpio_write
	li	a5,0
	mv	a0,a5
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (crosstool-NG esp-2022r1) 11.2.0"
