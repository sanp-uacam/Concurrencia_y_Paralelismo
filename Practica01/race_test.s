	.file	"race_test.c"
	.text
	.globl	global_counter
	.data
	.align 4
global_counter:
	.long	20
	.globl	lock
	.align 8
lock:
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
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L3:
	leaq	lock(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_lock         
	call	pthread_mutex_lock         ; entra a la sección crítica
	movl	global_counter(%rip), %eax ; LEER
	addl	$1, %eax                   ; SUMAR
	movl	%eax, global_counter(%rip) ; GUARDAR
	leaq	lock(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_unlock ; sale de la sección crítica
	addq	$1, -8(%rbp)
.L2:
	cmpq	$999999, -8(%rbp)
	jbe	.L3
	movl	$0, %eax
	addq	$48, %rsp
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
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L6
.L7:
	leaq	lock(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_lock
	movl	global_counter(%rip), %eax ; LEER
	subl	$1, %eax					; RESTAR
	movl	%eax, global_counter(%rip) ; GUARDAR
	leaq	lock(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_unlock
	addq	$1, -8(%rbp)
.L6:
	cmpq	$999, -8(%rbp)
	jbe	.L7
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "Valor final del contador: %d\12\0"
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
	leaq	lock(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_destroy
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r4) 16.1.0"
	.def	pthread_mutex_lock;	.scl	2;	.type	32;	.endef
	.def	pthread_mutex_unlock;	.scl	2;	.type	32;	.endef
	.def	pthread_create;	.scl	2;	.type	32;	.endef
	.def	pthread_join;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	pthread_mutex_destroy;	.scl	2;	.type	32;	.endef
