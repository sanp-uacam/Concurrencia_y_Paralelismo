	.file	"Seccion_critica.c"
	.text
	.globl	contador_compartido
	.data
	.align 4
contador_compartido:
	.long	20
	.globl	cerrojo_sincronizacion
	.align 8
cerrojo_sincronizacion:
	.quad	-1
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "C:/Users/Games/Documents/Quinto semestre/CONCURRENCIA Y PARALELISMO/ARCHIVO.txt\0"
	.text
	.globl	rutina_hilo_suma
	.def	rutina_hilo_suma;	.scl	2;	.type	32;	.endef
	.seh_proc	rutina_hilo_suma
rutina_hilo_suma:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$80, %rsp
	.seh_stackalloc	80
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movabsq	$8234092159644232040, %rax
	movabsq	$7308620121570043234, %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movabsq	$7525306209619242351, %rax
	movabsq	$2949325947761263, %rdx
	movq	%rax, -35(%rbp)
	movq	%rdx, -27(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L3:
	leaq	cerrojo_sincronizacion(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_lock
	movl	contador_compartido(%rip), %eax
	addl	$1, %eax
	movl	%eax, contador_compartido(%rip)
	leaq	cerrojo_sincronizacion(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_unlock
	leaq	.LC0(%rip), %rax
	movl	$9, %edx
	movq	%rax, %rcx
	call	open
	movl	%eax, -12(%rbp)
	leaq	-48(%rbp), %rdx
	movl	-12(%rbp), %eax
	movl	$28, %r8d
	movl	%eax, %ecx
	call	write
	movl	-12(%rbp), %eax
	movl	%eax, %ecx
	call	close
	addq	$1, -8(%rbp)
.L2:
	cmpq	$999, -8(%rbp)
	jbe	.L3
	movl	$0, %eax
	addq	$80, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	rutina_hilo_resta
	.def	rutina_hilo_resta;	.scl	2;	.type	32;	.endef
	.seh_proc	rutina_hilo_resta
rutina_hilo_resta:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movabsq	$2850150756936040, %rax
	movq	%rax, -20(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L6
.L7:
	leaq	cerrojo_sincronizacion(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_lock
	movl	contador_compartido(%rip), %eax
	subl	$1, %eax
	movl	%eax, contador_compartido(%rip)
	leaq	cerrojo_sincronizacion(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_unlock
	leaq	.LC0(%rip), %rax
	movl	$9, %edx
	movq	%rax, %rcx
	call	open
	movl	%eax, -12(%rbp)
	leaq	-20(%rbp), %rdx
	movl	-12(%rbp), %eax
	movl	$7, %r8d
	movl	%eax, %ecx
	call	write
	movl	-12(%rbp), %eax
	movl	%eax, %ecx
	call	close
	addq	$1, -8(%rbp)
.L6:
	cmpq	$999, -8(%rbp)
	jbe	.L7
	movl	$0, %eax
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC1:
	.ascii "Valor final de contador_compartido: %d\12\0"
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
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	call	__main
	leaq	rutina_hilo_suma(%rip), %rdx
	leaq	-8(%rbp), %rax
	movl	$0, %r9d
	movq	%rdx, %r8
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_create
	testl	%eax, %eax
	je	.L10
	movl	$-1, %eax
	jmp	.L13
.L10:
	leaq	rutina_hilo_resta(%rip), %rdx
	leaq	-16(%rbp), %rax
	movl	$0, %r9d
	movq	%rdx, %r8
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_create
	testl	%eax, %eax
	je	.L12
	movl	$-1, %eax
	jmp	.L13
.L12:
	movq	-8(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_join
	movq	-16(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rcx
	call	pthread_join
	movl	contador_compartido(%rip), %eax
	leaq	.LC1(%rip), %rcx
	movl	%eax, %edx
	call	__mingw_printf
	leaq	cerrojo_sincronizacion(%rip), %rax
	movq	%rax, %rcx
	call	pthread_mutex_destroy
	movl	$0, %eax
.L13:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev3, Built by MSYS2 project) 16.2.0"
	.def	pthread_mutex_lock;	.scl	2;	.type	32;	.endef
	.def	pthread_mutex_unlock;	.scl	2;	.type	32;	.endef
	.def	open;	.scl	2;	.type	32;	.endef
	.def	write;	.scl	2;	.type	32;	.endef
	.def	close;	.scl	2;	.type	32;	.endef
	.def	pthread_create;	.scl	2;	.type	32;	.endef
	.def	pthread_join;	.scl	2;	.type	32;	.endef
	.def	pthread_mutex_destroy;	.scl	2;	.type	32;	.endef
