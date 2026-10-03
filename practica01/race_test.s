	.file	"race_test.c"
# GNU C17 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O0 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.globl	global_counter
	.bss
	.align 8
	.type	global_counter, @object
	.size	global_counter, 8
global_counter:
	.zero	8
	.text
	.globl	incrementar
	.type	incrementar, @function
incrementar:
.LFB0:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)	# arg, arg
# race_test.c:10:     for (long i = 0; i < ITERACIONES; i++) {
	movq	$0, -8(%rbp)	#, i
# race_test.c:10:     for (long i = 0; i < ITERACIONES; i++) {
	jmp	.L2	#
.L3:
# race_test.c:11:         global_counter++;   // sección crítica sin protección
	movq	global_counter(%rip), %rax	# global_counter, global_counter.0_1
	addq	$1, %rax	#, _2
	movq	%rax, global_counter(%rip)	# _2, global_counter
# race_test.c:10:     for (long i = 0; i < ITERACIONES; i++) {
	addq	$1, -8(%rbp)	#, i
.L2:
# race_test.c:10:     for (long i = 0; i < ITERACIONES; i++) {
	cmpq	$999999, -8(%rbp)	#, i
	jle	.L3	#,
# race_test.c:13:     return NULL;
	movl	$0, %eax	#, _7
# race_test.c:14: }
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE0:
	.size	incrementar, .-incrementar
	.section	.rodata
.LC0:
	.string	"Valor esperado : %ld\n"
.LC1:
	.string	"Valor obtenido : %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$48, %rsp	#,
# race_test.c:16: int main(void) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp97
	movq	%rax, -8(%rbp)	# tmp97, D.3813
	xorl	%eax, %eax	# tmp97
# race_test.c:19:     for (int i = 0; i < NUM_HILOS; i++) {
	movl	$0, -40(%rbp)	#, i
# race_test.c:19:     for (int i = 0; i < NUM_HILOS; i++) {
	jmp	.L6	#
.L7:
# race_test.c:20:         pthread_create(&hilos[i], NULL, incrementar, NULL);
	leaq	-32(%rbp), %rax	#, tmp87
	movl	-40(%rbp), %edx	# i, tmp89
	movslq	%edx, %rdx	# tmp89, tmp88
	salq	$3, %rdx	#, tmp90
	addq	%rdx, %rax	# tmp90, _1
	movl	$0, %ecx	#,
	leaq	incrementar(%rip), %rdx	#, tmp91
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _1,
	call	pthread_create@PLT	#
# race_test.c:19:     for (int i = 0; i < NUM_HILOS; i++) {
	addl	$1, -40(%rbp)	#, i
.L6:
# race_test.c:19:     for (int i = 0; i < NUM_HILOS; i++) {
	cmpl	$1, -40(%rbp)	#, i
	jle	.L7	#,
# race_test.c:23:     for (int i = 0; i < NUM_HILOS; i++) {
	movl	$0, -36(%rbp)	#, i
# race_test.c:23:     for (int i = 0; i < NUM_HILOS; i++) {
	jmp	.L8	#
.L9:
# race_test.c:24:         pthread_join(hilos[i], NULL);
	movl	-36(%rbp), %eax	# i, tmp93
	cltq
	movq	-32(%rbp,%rax,8), %rax	# hilos[i_5], _2
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _2,
	call	pthread_join@PLT	#
# race_test.c:23:     for (int i = 0; i < NUM_HILOS; i++) {
	addl	$1, -36(%rbp)	#, i
.L8:
# race_test.c:23:     for (int i = 0; i < NUM_HILOS; i++) {
	cmpl	$1, -36(%rbp)	#, i
	jle	.L9	#,
# race_test.c:27:     printf("Valor esperado : %ld\n", (long)NUM_HILOS * ITERACIONES);
	movl	$2000000, %esi	#,
	leaq	.LC0(%rip), %rax	#, tmp94
	movq	%rax, %rdi	# tmp94,
	movl	$0, %eax	#,
	call	printf@PLT	#
# race_test.c:28:     printf("Valor obtenido : %ld\n", global_counter);
	movq	global_counter(%rip), %rax	# global_counter, global_counter.1_3
	movq	%rax, %rsi	# global_counter.1_3,
	leaq	.LC1(%rip), %rax	#, tmp95
	movq	%rax, %rdi	# tmp95,
	movl	$0, %eax	#,
	call	printf@PLT	#
# race_test.c:30:     return 0;
	movl	$0, %eax	#, _13
# race_test.c:31: }
	movq	-8(%rbp), %rdx	# D.3813, tmp98
	subq	%fs:40, %rdx	# MEM[(<address-space-1> long unsigned int *)40B], tmp98
	je	.L11	#,
	call	__stack_chk_fail@PLT	#
.L11:
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE1:
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
