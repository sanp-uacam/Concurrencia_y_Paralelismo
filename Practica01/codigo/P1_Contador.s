	.file	"P1_Contador.c"
	.text
	.globl	global_counter
	.data
	.align 4
	.type	global_counter, @object
	.size	global_counter, 4
global_counter:
	.long	20
	.globl	mutex
	.bss
	.align 32
	.type	mutex, @object
	.size	mutex, 40
mutex:
	.zero	40
	.section	.rodata
.LC0:
	.string	"starting thread..."
.LC1:
	.string	"Thread one: %d + 1 = %d\n"
	.align 8
.LC2:
	.string	"/home/lucyjy/CyP_prueba/Practica1/readme.txt"
	.text
	.globl	thread_routine
	.type	thread_routine, @function
thread_routine:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$144, %rsp
	movq	%rdi, -136(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-136(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -120(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -124(%rbp)
	jmp	.L2
.L4:
	leaq	mutex(%rip), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock@PLT
	movl	global_counter(%rip), %eax
	leal	1(%rax), %ecx
	movl	global_counter(%rip), %edx
	leaq	-112(%rbp), %rax
	movl	%ecx, %r8d
	movl	%edx, %ecx
	leaq	.LC1(%rip), %rdx
	movl	$100, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	movl	global_counter(%rip), %eax
	addl	$1, %eax
	movl	%eax, global_counter(%rip)
	movl	$420, %edx
	movl	$1089, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -116(%rbp)
	cmpl	$-1, -116(%rbp)
	je	.L3
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rdx
	leaq	-112(%rbp), %rcx
	movl	-116(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	movl	-116(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
.L3:
	leaq	mutex(%rip), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock@PLT
	addl	$1, -124(%rbp)
.L2:
	movl	-124(%rbp), %eax
	cmpl	-120(%rbp), %eax
	jl	.L4
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	thread_routine, .-thread_routine
	.section	.rodata
.LC3:
	.string	"starting thread two... "
.LC4:
	.string	"Thread two: %d - 1 = %d\n"
	.text
	.globl	thread_routine_two
	.type	thread_routine_two, @function
thread_routine_two:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$144, %rsp
	movq	%rdi, -136(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-136(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -120(%rbp)
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -124(%rbp)
	jmp	.L8
.L10:
	leaq	mutex(%rip), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock@PLT
	movl	global_counter(%rip), %eax
	leal	-1(%rax), %ecx
	movl	global_counter(%rip), %edx
	leaq	-112(%rbp), %rax
	movl	%ecx, %r8d
	movl	%edx, %ecx
	leaq	.LC4(%rip), %rdx
	movl	$100, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	movl	global_counter(%rip), %eax
	subl	$1, %eax
	movl	%eax, global_counter(%rip)
	movl	$420, %edx
	movl	$1089, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -116(%rbp)
	cmpl	$-1, -116(%rbp)
	je	.L9
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rdx
	leaq	-112(%rbp), %rcx
	movl	-116(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	movl	-116(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
.L9:
	leaq	mutex(%rip), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock@PLT
	addl	$1, -124(%rbp)
.L8:
	movl	-124(%rbp), %eax
	cmpl	-120(%rbp), %eax
	jl	.L10
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L12
	call	__stack_chk_fail@PLT
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	thread_routine_two, .-thread_routine_two
	.section	.rodata
.LC5:
	.string	"Uso: %s <numero_de_lineas>\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$1, -36(%rbp)
	jg	.L14
	movq	-48(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %eax
	jmp	.L18
.L14:
	movq	-48(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -28(%rbp)
	leaq	-28(%rbp), %rdx
	leaq	-24(%rbp), %rax
	movq	%rdx, %rcx
	leaq	thread_routine(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	je	.L16
	movl	$-1, %eax
	jmp	.L18
.L16:
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rax
	movq	%rdx, %rcx
	leaq	thread_routine_two(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	je	.L17
	movl	$-1, %eax
	jmp	.L18
.L17:
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	$0, %eax
.L18:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L19
	call	__stack_chk_fail@PLT
.L19:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
