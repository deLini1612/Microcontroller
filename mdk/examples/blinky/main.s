	.file	"main.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.type	spin, @function
spin:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	j	.L2
.L3:
 #APP
# 63 "/home/nplink/Documents/VXL/mdk/esp32c3/mdk.h" 1
	nop
# 0 "" 2
 #NO_APP
.L2:
	lw	a5,-20(s0)
	addi	a4,a5,-1
	sw	a4,-20(s0)
	bne	a5,zero,.L3
	nop
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	spin, .-spin
	.align	1
	.type	systick, @function
systick:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	sw	s2,36(sp)
	sw	s3,32(sp)
	sw	s4,28(sp)
	sw	s5,24(sp)
	sw	s6,20(sp)
	sw	s7,16(sp)
	sw	s8,12(sp)
	sw	s9,8(sp)
	addi	s0,sp,48
	li	a5,1610756096
	addi	a5,a5,4
	li	a4,1073741824
	sw	a4,0(a5)
	li	a0,1
	call	spin
	li	a5,1610756096
	addi	a5,a5,64
	lw	a5,0(a5)
	mv	s8,a5
	li	s9,0
	slli	s3,s8,0
	li	s2,0
	li	a5,1610756096
	addi	a5,a5,68
	lw	a5,0(a5)
	mv	s4,a5
	li	s5,0
	or	s6,s2,s4
	or	s7,s3,s5
	mv	a4,s6
	mv	a5,s7
	mv	a0,a4
	mv	a1,a5
	lw	ra,44(sp)
	lw	s0,40(sp)
	lw	s2,36(sp)
	lw	s3,32(sp)
	lw	s4,28(sp)
	lw	s5,24(sp)
	lw	s6,20(sp)
	lw	s7,16(sp)
	lw	s8,12(sp)
	lw	s9,8(sp)
	addi	sp,sp,48
	jr	ra
	.size	systick, .-systick
	.align	1
	.type	uptime_us, @function
uptime_us:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s2,4(sp)
	sw	s3,0(sp)
	addi	s0,sp,16
	call	systick
	mv	a4,a0
	mv	a5,a1
	slli	a3,a5,28
	srli	s2,a4,4
	or	s2,a3,s2
	srli	s3,a5,4
	mv	a4,s2
	mv	a5,s3
	mv	a0,a4
	mv	a1,a5
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s2,4(sp)
	lw	s3,0(sp)
	addi	sp,sp,16
	jr	ra
	.size	uptime_us, .-uptime_us
	.align	1
	.type	delay_us, @function
delay_us:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	sw	s2,36(sp)
	sw	s3,32(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	call	uptime_us
	mv	a2,a0
	mv	a3,a1
	lw	a5,-36(s0)
	mv	s2,a5
	li	s3,0
	add	a4,a2,s2
	mv	a1,a4
	sltu	a1,a1,a2
	add	a5,a3,s3
	add	a3,a1,a5
	mv	a5,a3
	sw	a4,-24(s0)
	sw	a5,-20(s0)
	j	.L9
.L10:
	li	a0,1
	call	spin
.L9:
	call	uptime_us
	mv	a4,a0
	mv	a5,a1
	lw	a3,-20(s0)
	mv	a2,a5
	bgtu	a3,a2,.L10
	lw	a3,-20(s0)
	mv	a2,a5
	bne	a3,a2,.L12
	lw	a3,-24(s0)
	mv	a5,a4
	bgtu	a3,a5,.L10
.L12:
	nop
	lw	ra,44(sp)
	lw	s0,40(sp)
	lw	s2,36(sp)
	lw	s3,32(sp)
	addi	sp,sp,48
	jr	ra
	.size	delay_us, .-delay_us
	.align	1
	.type	delay_ms, @function
delay_ms:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lw	a4,-20(s0)
	li	a5,1000
	mul	a5,a4,a5
	mv	a0,a5
	call	delay_us
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	delay_ms, .-delay_ms
	.align	1
	.type	wdt_disable, @function
wdt_disable:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,1610645504
	addi	a5,a5,168
	li	a4,1356349440
	addi	a4,a4,-1375
	sw	a4,0(a5)
	li	a5,1610645504
	addi	a5,a5,144
	sw	zero,0(a5)
	li	a5,1610645504
	addi	a5,a5,140
	sw	zero,0(a5)
	li	a5,1610645504
	addi	a5,a5,176
	li	a4,-1893912576
	addi	a4,a4,298
	sw	a4,0(a5)
	li	a5,1610645504
	addi	a5,a5,172
	lw	a3,0(a5)
	li	a5,1610645504
	addi	a5,a5,172
	li	a4,-2147483648
	or	a4,a3,a4
	sw	a4,0(a5)
	li	a5,1610645504
	addi	a5,a5,180
	sw	zero,0(a5)
	li	a5,1610739712
	addi	a5,a5,252
	lw	a4,0(a5)
	li	a5,1610739712
	addi	a5,a5,252
	andi	a4,a4,-513
	sw	a4,0(a5)
	li	a5,1610739712
	addi	a5,a5,72
	sw	zero,0(a5)
	li	a5,1610743808
	addi	a5,a5,72
	sw	zero,0(a5)
	nop
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	wdt_disable, .-wdt_disable
	.align	1
	.type	gpio_output_enable, @function
gpio_output_enable:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	mv	a5,a1
	sb	a5,-21(s0)
	li	a5,1610629120
	addi	a5,a5,32
	lw	a3,0(a5)
	lw	a5,-20(s0)
	li	a4,1
	sll	a5,a4,a5
	not	a4,a5
	li	a5,1610629120
	addi	a5,a5,32
	and	a4,a3,a4
	sw	a4,0(a5)
	li	a5,1610629120
	addi	a5,a5,32
	lw	a3,0(a5)
	lbu	a5,-21(s0)
	beq	a5,zero,.L16
	li	a5,1
	j	.L17
.L16:
	li	a5,0
.L17:
	lw	a4,-20(s0)
	sll	a4,a5,a4
	li	a5,1610629120
	addi	a5,a5,32
	or	a4,a3,a4
	sw	a4,0(a5)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	gpio_output_enable, .-gpio_output_enable
	.align	1
	.type	gpio_output, @function
gpio_output:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lw	a5,-20(s0)
	addi	a5,a5,341
	slli	a4,a5,2
	li	a5,1610629120
	add	a5,a4,a5
	li	a4,640
	sw	a4,0(a5)
	li	a1,1
	lw	a0,-20(s0)
	call	gpio_output_enable
	nop
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	gpio_output, .-gpio_output
	.align	1
	.type	gpio_write, @function
gpio_write:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	mv	a5,a1
	sb	a5,-21(s0)
	li	a5,1610629120
	addi	a5,a5,4
	lw	a3,0(a5)
	lw	a5,-20(s0)
	li	a4,1
	sll	a5,a4,a5
	not	a4,a5
	li	a5,1610629120
	addi	a5,a5,4
	and	a4,a3,a4
	sw	a4,0(a5)
	li	a5,1610629120
	addi	a5,a5,4
	lw	a3,0(a5)
	lbu	a5,-21(s0)
	beq	a5,zero,.L20
	li	a5,1
	j	.L21
.L20:
	li	a5,0
.L21:
	lw	a4,-20(s0)
	sll	a4,a5,a4
	li	a5,1610629120
	addi	a5,a5,4
	or	a4,a3,a4
	sw	a4,0(a5)
	nop
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	gpio_write, .-gpio_write
	.section	.sdata,"aw"
	.align	2
	.type	led_pin, @object
	.size	led_pin, 4
led_pin:
	.word	2
	.local	led_state
	.comm	led_state,4,4
	.section	.rodata
	.align	2
.LC0:
	.string	"LED: %d\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	addi	s0,sp,16
	lui	a5,%hi(led_pin)
	lw	a5,%lo(led_pin)(a5)
	mv	a0,a5
	call	gpio_output
	call	wdt_disable
.L23:
	lui	a5,%hi(led_state)
	lw	a5,%lo(led_state)(a5)
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	lui	a5,%hi(led_pin)
	lw	a4,%lo(led_pin)(a5)
	lui	a5,%hi(led_state)
	lw	a5,%lo(led_state)(a5)
	snez	a5,a5
	andi	a5,a5,0xff
	mv	a1,a5
	mv	a0,a4
	call	gpio_write
	lui	a5,%hi(led_state)
	lw	a5,%lo(led_state)(a5)
	seqz	a5,a5
	andi	a5,a5,0xff
	mv	a4,a5
	lui	a5,%hi(led_state)
	sw	a4,%lo(led_state)(a5)
	li	a0,888
	call	delay_ms
	j	.L23
	.size	main, .-main
	.ident	"GCC: (crosstool-NG esp-2022r1) 11.2.0"
