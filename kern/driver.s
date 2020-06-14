	.file	"driver.c"
	.text
	.globl	inp
	.type	inp, @function
inp:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %ecx
#APP
# 21 "driver.c" 1
	mov %ecx, %edx
	in %dx, %al
	movl %eax, %ecx
	
# 0 "" 2
#NO_APP
	movl	%ecx, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	inp, .-inp
	.globl	outp
	.type	outp, @function
outp:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %ecx
	movl	-8(%rbp), %esi
#APP
# 31 "driver.c" 1
	movl %ecx, %edx
	movl %esi, %eax
	out %al, %dx


# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	outp, .-outp
	.globl	insl
	.type	insl, @function
insl:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movw	%ax, -4(%rbp)
	movzwl	-4(%rbp), %eax
	movq	-16(%rbp), %rdx
	movq	-24(%rbp), %rcx
#APP
# 41 "driver.c" 1
	cld
	movq %rcx, %rcx
	movq %rdx, %rdi
	movw %ax, %dx
	rep insl
	
# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	insl, .-insl
	.globl	SetInterupt
	.type	SetInterupt, @function
SetInterupt:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movq	%rsi, -32(%rbp)
	movw	%ax, -20(%rbp)
#APP
# 69 "driver.c" 1
	cli
	
# 0 "" 2
#NO_APP
	movq	$1048576, -16(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -8(%rbp)
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, 12(%rax)
	movq	-8(%rbp), %rax
	shrq	$32, %rax
	movq	%rax, %rdx
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rcx
	movq	-16(%rbp), %rax
	addq	%rcx, %rax
	movl	%edx, 8(%rax)
	movq	-8(%rbp), %rax
	shrq	$16, %rax
	movq	%rax, %rdx
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rcx
	movq	-16(%rbp), %rax
	addq	%rcx, %rax
	movw	%dx, 6(%rax)
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movq	-8(%rbp), %rdx
	movw	%dx, (%rax)
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movw	$8, 2(%rax)
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movw	$-29184, 4(%rax)
	movzwl	-20(%rbp), %eax
	salq	$4, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, 12(%rax)
#APP
# 79 "driver.c" 1
	sti
	
# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	SetInterupt, .-SetInterupt
	.data
	.align 32
	.type	normalmap, @object
	.size	normalmap, 256
normalmap:
	.string	""
	.string	"\0331234567890-=\b\tqwertyuiop[]\n"
	.string	"asdfghjkl;'`"
	.string	"\\zxcvbnm,./"
	.string	"*"
	.string	" "
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"789-456+1230."
	.string	""
	.string	""
	.string	""
	.string	""
	.zero	167
	.align 32
	.type	shiftmap, @object
	.size	shiftmap, 256
shiftmap:
	.string	""
	.string	"\033!@#$%^&*()_+\b\tQWERTYUIOP{}\n"
	.string	"ASDFGHJKL:\"~"
	.string	"|ZXCVBNM<>?"
	.string	"*"
	.string	" "
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"789-456+1230."
	.string	""
	.string	""
	.string	""
	.string	""
	.zero	167
	.globl	inputbuf
	.bss
	.align 32
	.type	inputbuf, @object
	.size	inputbuf, 32
inputbuf:
	.zero	32
	.globl	output
	.type	output, @object
	.size	output, 1
output:
	.zero	1
	.text
	.globl	KbHandler
	.type	KbHandler, @function
KbHandler:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
#APP
# 123 "driver.c" 1
	push %rax
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	
# 0 "" 2
#NO_APP
	movl	$96, %edi
	call	inp
	movb	%al, -2(%rbp)
	movzbl	-2(%rbp), %eax
	testb	%al, %al
	js	.L7
	movzbl	-2(%rbp), %eax
	cltq
	leaq	normalmap(%rip), %rdx
	movzbl	(%rax,%rdx), %eax
	movb	%al, -1(%rbp)
	movzbl	-1(%rbp), %eax
	movb	%al, output(%rip)
.L7:
#APP
# 130 "driver.c" 1
	movb $0x20, %al
	
# 0 "" 2
# 130 "driver.c" 1
	out %al, $0x20
	
# 0 "" 2
# 131 "driver.c" 1
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %rax
	
# 0 "" 2
# 132 "driver.c" 1
	nop
	leaveq
	iretq
	
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	KbHandler, .-KbHandler
	.section	.rodata
.LC0:
	.string	"Ouch! Ouch!"
	.text
	.globl	Ouch
	.type	Ouch, @function
Ouch:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
#APP
# 136 "driver.c" 1
	push %rax
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	
# 0 "" 2
#NO_APP
	movl	$96, %edi
	call	inp
	movb	%al, -1(%rbp)
	movzbl	-1(%rbp), %eax
	testb	%al, %al
	js	.L9
	movl	4+cur.1634(%rip), %eax
	leal	2(%rax), %ecx
	movslq	%ecx, %rax
	imulq	$715827883, %rax, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	%edx, 4+cur.1634(%rip)
	movl	cur.1634(%rip), %eax
	leal	-2(%rax), %ecx
	movslq	%ecx, %rax
	imulq	$1717986919, %rax, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	sarl	$5, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	%edx, cur.1634(%rip)
	movq	cur.1634(%rip), %rax
	movq	%rax, %rdi
	call	SetCurPos
	leaq	.LC0(%rip), %rdi
	call	puts
.L9:
#APP
# 145 "driver.c" 1
	movb $0x20, %al
	
# 0 "" 2
# 145 "driver.c" 1
	out %al, $0x20
	
# 0 "" 2
# 146 "driver.c" 1
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %rax
	
# 0 "" 2
# 147 "driver.c" 1
	nop
	leaveq
	iretq
	
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	Ouch, .-Ouch
	.globl	SetOuch
	.type	SetOuch, @function
SetOuch:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	Ouch(%rip), %rsi
	movl	$33, %edi
	call	SetInterupt
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	SetOuch, .-SetOuch
	.globl	InitKeyboard
	.type	InitKeyboard, @function
InitKeyboard:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	KbHandler(%rip), %rsi
	movl	$33, %edi
	call	SetInterupt
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	InitKeyboard, .-InitKeyboard
	.globl	getchar
	.type	getchar, @function
getchar:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movb	$0, output(%rip)
	nop
.L13:
	movzbl	output(%rip), %eax
	testb	%al, %al
	jle	.L13
	movzbl	output(%rip), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putc
	movzbl	output(%rip), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	getchar, .-getchar
	.globl	gets
	.type	gets, @function
gets:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, %eax
	call	getchar
	movb	%al, -5(%rbp)
	jmp	.L16
.L17:
	movl	-4(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -4(%rbp)
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-5(%rbp), %eax
	movb	%al, (%rdx)
	movl	$0, %eax
	call	getchar
	movb	%al, -5(%rbp)
.L16:
	cmpb	$10, -5(%rbp)
	jne	.L17
	movl	$10, %edi
	call	putc
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	gets, .-gets
	.section	.rodata
.LC1:
	.string	"|/\\"
	.text
	.globl	CcHandler
	.type	CcHandler, @function
CcHandler:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rdi
	pushq	%rsi
	pushq	%rdx
	pushq	%rax
	.cfi_offset 5, -24
	.cfi_offset 4, -32
	.cfi_offset 1, -40
	.cfi_offset 0, -48
	leaq	8(%rbp), %rax
	movq	%rax, -56(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -48(%rbp)
	movq	$753664, -40(%rbp)
	movl	index.1661(%rip), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %edx
	movq	-40(%rbp), %rax
	addq	$3998, %rax
	movb	%dl, (%rax)
	movq	-40(%rbp), %rax
	addq	$3999, %rax
	movb	$48, (%rax)
	movl	index.1661(%rip), %eax
	leal	1(%rax), %esi
	movslq	%esi, %rax
	imulq	$1431655766, %rax, %rax
	shrq	$32, %rax
	movq	%rax, %rdx
	movl	%esi, %eax
	sarl	$31, %eax
	movl	%edx, %edi
	subl	%eax, %edi
	movl	%edi, %eax
	movl	%eax, %edx
	addl	%edx, %edx
	addl	%eax, %edx
	movl	%esi, %eax
	subl	%edx, %eax
	movl	%eax, index.1661(%rip)
#APP
# 200 "driver.c" 1
	movb $0x20, %al
	
# 0 "" 2
# 200 "driver.c" 1
	out %al, $0x20
	
# 0 "" 2
#NO_APP
	nop
	popq	%rax
	popq	%rdx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	.cfi_def_cfa 7, 8
	iretq
	.cfi_endproc
.LFE10:
	.size	CcHandler, .-CcHandler
	.globl	InitClock
	.type	InitClock, @function
InitClock:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	CcHandler(%rip), %rsi
	movl	$32, %edi
	call	SetInterupt
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	InitClock, .-InitClock
	.globl	GetCurPos
	.type	GetCurPos, @function
GetCurPos:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movw	$0, -10(%rbp)
	movl	$14, %esi
	movl	$980, %edi
	call	outp
	movl	$981, %edi
	call	inp
	sall	$8, %eax
	movb	$0, %al
	movw	%ax, -10(%rbp)
	movl	$15, %esi
	movl	$980, %edi
	call	outp
	movl	$981, %edi
	call	inp
	addw	%ax, -10(%rbp)
	movzwl	-10(%rbp), %ecx
	movzwl	%cx, %eax
	imull	$52429, %eax, %eax
	shrl	$16, %eax
	movl	%eax, %edx
	shrw	$6, %dx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movzwl	%dx, %eax
	movl	%eax, -8(%rbp)
	movzwl	-10(%rbp), %eax
	movzwl	%ax, %eax
	imull	$52429, %eax, %eax
	shrl	$16, %eax
	shrw	$6, %ax
	movzwl	%ax, %eax
	movl	%eax, -4(%rbp)
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	GetCurPos, .-GetCurPos
	.globl	SetCurPos
	.type	SetCurPos, @function
SetCurPos:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-24(%rbp), %eax
	addl	%edx, %eax
	movw	%ax, -2(%rbp)
	movl	$14, %esi
	movl	$980, %edi
	call	outp
	movzwl	-2(%rbp), %eax
	shrw	$8, %ax
	movzwl	%ax, %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	movl	$981, %edi
	call	outp
	movl	$15, %esi
	movl	$980, %edi
	call	outp
	movzwl	-2(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	movl	$981, %edi
	call	outp
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	SetCurPos, .-SetCurPos
	.globl	ScrollUp
	.type	ScrollUp, @function
ScrollUp:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$0, %eax
	call	GetCurPos
	movq	%rax, -8(%rbp)
	movq	$753664, -16(%rbp)
	movl	$0, -24(%rbp)
	jmp	.L24
.L25:
	movl	-24(%rbp), %eax
	cltq
	addq	$80, %rax
	leaq	(%rax,%rax), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	-24(%rbp), %edx
	movslq	%edx, %rdx
	leaq	(%rdx,%rdx), %rcx
	movq	-16(%rbp), %rdx
	addq	%rcx, %rdx
	movzwl	(%rax), %eax
	movw	%ax, (%rdx)
	addl	$1, -24(%rbp)
.L24:
	cmpl	$1918, -24(%rbp)
	jle	.L25
	movl	$1920, -20(%rbp)
	jmp	.L26
.L27:
	movl	-20(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movw	$12320, (%rax)
	addl	$1, -20(%rbp)
.L26:
	cmpl	$1999, -20(%rbp)
	jle	.L27
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	SetCurPos
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	ScrollUp, .-ScrollUp
	.globl	putc
	.type	putc, @function
putc:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movl	%edi, %eax
	movb	%al, -36(%rbp)
	movq	$753664, -16(%rbp)
	movl	$0, %eax
	call	GetCurPos
	movq	%rax, -8(%rbp)
	cmpb	$10, -36(%rbp)
	jne	.L29
	movl	$0, -8(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.L30
.L29:
	movl	-4(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %edx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-36(%rbp), %eax
	movb	%al, (%rdx)
	movl	-20(%rbp), %eax
	addl	$1, %eax
	movl	%eax, %edx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movb	$49, (%rax)
	movl	-8(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rbp)
.L30:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	SetCurPos
	movl	-4(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	cmpl	$1999, %eax
	jle	.L32
	movl	$0, %eax
	call	ScrollUp
.L32:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	putc, .-putc
	.globl	puts
	.type	puts, @function
puts:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L34
.L35:
	movl	-4(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -4(%rbp)
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putc
.L34:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L35
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	puts, .-puts
	.globl	WaitDisk
	.type	WaitDisk, @function
WaitDisk:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
.L37:
	movl	$503, %edi
	call	inp
	andl	$192, %eax
	cmpl	$64, %eax
	jne	.L37
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	WaitDisk, .-WaitDisk
	.globl	readsect
	.type	readsect, @function
readsect:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, %eax
	movw	%ax, -16(%rbp)
	movl	$0, %eax
	call	WaitDisk
	movswl	-16(%rbp), %eax
	movl	%eax, %esi
	movl	$498, %edi
	call	outp
	movl	-12(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	movl	$499, %edi
	call	outp
	movl	-12(%rbp), %eax
	sarl	$8, %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	movl	$500, %edi
	call	outp
	movl	-12(%rbp), %eax
	sarl	$16, %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	movl	$501, %edi
	call	outp
	movl	-12(%rbp), %eax
	sarl	$24, %eax
	andl	$15, %eax
	orb	$-32, %al
	movl	%eax, %esi
	movl	$502, %edi
	call	outp
	movl	$32, %esi
	movl	$503, %edi
	call	outp
	movl	$0, %eax
	call	WaitDisk
	movswl	-16(%rbp), %eax
	sall	$7, %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$496, %edi
	call	insl
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	readsect, .-readsect
	.data
	.align 8
	.type	cur.1634, @object
	.size	cur.1634, 8
cur.1634:
	.long	55
	.long	17
	.local	index.1661
	.comm	index.1661,4,4
	.ident	"GCC: (Ubuntu 9.3.0-11ubuntu0~18.04.1) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
