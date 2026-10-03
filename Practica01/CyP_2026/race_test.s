	.file	"race_test.c"
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
	.string	"Starting thread..."
	.align 8
.LC1:
	.string	"/mnt/c/Users/glady/Onedrive/Desktop/CyP_2026/readme.txt"
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
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -28(%rbp)
	movabsq	$7308895158390973774, %rax
	movq	%rax, -18(%rbp)
	movw	$10, -10(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -32(%rbp)
	jmp	.L2
.L3:
	movl	global_counter(%rip), %eax
	addl	$1, %eax
	movl	%eax, global_counter(%rip)
	leaq	.LC1(%rip), %rax
	movl	$1025, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -24(%rbp)
	leaq	-18(%rbp), %rcx
	movl	-24(%rbp), %eax
	movl	$9, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	movl	-24(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
	addl	$1, -32(%rbp)
.L2:
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L3
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	thread_routine, .-thread_routine
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
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -28(%rbp)
	movabsq	$3327724139885718599, %rax
	movq	%rax, -19(%rbp)
	movl	$667182, -12(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -32(%rbp)
	jmp	.L7
.L8:
	movl	global_counter(%rip), %eax
	subl	$1, %eax
	movl	%eax, global_counter(%rip)
	leaq	.LC1(%rip), %rax
	movl	$1025, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -24(%rbp)
	leaq	-19(%rbp), %rcx
	movl	-24(%rbp), %eax
	movl	$10, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	movl	-24(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
	addl	$1, -32(%rbp)
.L7:
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L8
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L10
	call	__stack_chk_fail@PLT
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	thread_routine_two, .-thread_routine_two
	.section	.rodata
.LC2:
	.string	"Contador final: %d\n"
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
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1000, -28(%rbp)
	leaq	-28(%rbp), %rdx
	leaq	thread_routine(%rip), %rsi
	leaq	-24(%rbp), %rax
	movq	%rdx, %rcx
	movq	%rsi, %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	leaq	-28(%rbp), %rdx
	leaq	thread_routine_two(%rip), %rsi
	leaq	-16(%rbp), %rax
	movq	%rdx, %rcx
	movq	%rsi, %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	global_counter(%rip), %eax
	leaq	.LC2(%rip), %rdx
	movl	%eax, %esi
	movq	%rdx, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L13
	call	__stack_chk_fail@PLT
.L13:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 15.2.0-16ubuntu1) 15.2.0"
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
