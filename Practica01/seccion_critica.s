	.file	"seccion_critica.c"
	.text
	.globl	global_counter
	.data
	.align 4
global_counter:
	.long	20
	.globl	mutex
	.align 8
mutex:
	.quad	-1
	.text
	.globl	thread_routine
	.def	thread_routine;	.scl	2;	.type	32;	.endef
	.seh_proc	thread_routine
thread_routine:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L3:
	movl	global_counter(%rip), %eax
	addl	$1, %eax
	movl	%eax, global_counter(%rip)
	addq	$1, -8(%rbp)
.L2:
	cmpq	$999, -8(%rbp)
	jbe	.L3
	nop
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	thread_routine_two
	.def	thread_routine_two;	.scl	2;	.type	32;	.endef
	.seh_proc	thread_routine_two
thread_routine_two:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L5
.L6:
	movl	global_counter(%rip), %eax
	subl	$1, %eax
	movl	%eax, global_counter(%rip)
	addq	$1, -8(%rbp)
.L5:
	cmpq	$999, -8(%rbp)
	jbe	.L6
	movl	$0, %eax
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "Valor final de global_counter: %d\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	leaq	thread_routine(%rip), %rdx
	leaq	-8(%rbp), %rax
	movl	$0, %r9d
	movq	%rdx, %r8
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_create
	leaq	thread_routine_two(%rip), %rdx
	leaq	-16(%rbp), %rax
	movl	$0, %r9d
	movq	%rdx, %r8
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_create
	movq	-8(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_join
	movq	-16(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_join
	movl	global_counter(%rip), %eax
	leaq	.LC0(%rip), %rcx
	movl	%eax, %edx
	call	printf
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev5, Built by MSYS2 project) 16.1.0"
	.def	pthread_create;	.scl	2;	.type	32;	.endef
	.def	pthread_join;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
