.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 13, 1

# ==============================================================================
# FUNCIÓN: thread_routine
# ==============================================================================
	.globl	_thread_routine                 ## Hace la función visible globalmente
	.p2align	4, 0x90
_thread_routine:                            ## Punto de entrada de la función
	.cfi_startproc
## %bb.0:
	pushq	%rbp                            # Guarda el Base Pointer anterior en el stack
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp                      # Establece el nuevo Frame Pointer
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp                       # Reserva 48 bytes en el stack para variables locales

	# --- Protección del Stack (Stack Canary Guard) ---
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)                  # Guarda el canario en -8(%rbp)

	movq	%rdi, -32(%rbp)                 # Guarda el parámetro de entrada (void* arg)

	# --- Inicialización de la cadena local "New line \n" (10 bytes) ---
	movq	L___const.thread_routine.buf(%rip), %rax
	movq	%rax, -19(%rbp)
	movw	L___const.thread_routine.buf+8(%rip), %ax
	movw	%ax, -11(%rbp)
	movb	L___const.thread_routine.buf+10(%rip), %al
	movb	%al, -9(%rbp)

	movq	$0, -48(%rbp)                   # Inicializa la variable del bucle i = 0

LBB0_1:                                     ## Encabezado del bucle (for i < 1000)
	cmpq	$1000, -48(%rbp)                # Compara i con 1000
	jae	LBB0_4                              # Si i >= 1000, sale del bucle hacia LBB0_4

## %bb.2:                                   ## Cuerpo del bucle
	# --- SECCIÓN CRÍTICA: Incremento protegido por Mutex ---
	leaq	_mutex(%rip), %rdi              # Carga la dirección del mutex en %rdi
	callq	_pthread_mutex_lock             # Bloquea el mutex

	movl	_global_counter(%rip), %eax     # Lee _global_counter desde memoria a %eax
	addl	$1, %eax                        # Suma 1 a %eax (INCREMENTO)
	movl	%eax, _global_counter(%rip)     # Escribe el nuevo valor en memoria

	leaq	_mutex(%rip), %rdi              # Carga la dirección del mutex en %rdi
	callq	_pthread_mutex_unlock           # Libera el mutex

	# --- Operación I/O: Escritura en archivo ---
	leaq	L_.str(%rip), %rdi              # Carga la ruta del archivo "/Users/vdj/Desktop/Cyp_2026/README.txt"
	movl	$9, %esi                        # Modo de apertura (O_WRONLY | O_CREAT | O_APPEND = 0x0009)
	movb	$0, %al                         # 0 argumentos en registros varargs
	callq	_open                           # Llama a open(...)
	movl	%eax, -36(%rbp)                 # Guarda el descriptor de archivo (fd) resultante

	movl	-36(%rbp), %edi                 # 1er arg: fd
	leaq	-19(%rbp), %rsi                 # 2do arg: buffer ("New line \n")
	movl	$10, %edx                       # 3er arg: número de bytes (10)
	callq	_write                          # Llama a write(fd, buf, 10)

	movl	-36(%rbp), %edi                 # 1er arg: fd
	callq	_close                          # Cierra el archivo con close(fd)

## %bb.3:                                   ## Incremento del bucle
	movq	-48(%rbp), %rax
	addq	$1, %rax                        # i++
	movq	%rax, -48(%rbp)
	jmp	LBB0_1                          # Salto al inicio del bucle

LBB0_4:                                     ## Salida de la función y verificación de Stack
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rcx
	cmpq	%rcx, %rax                      # Verifica que el canario no haya sido alterado
	jne	LBB0_6

## %bb.5:
	xorl	%eax, %eax                      # Retorna NULL (0 en %rax)
	addq	$48, %rsp                       # Libera el espacio del stack
	popq	%rbp                            # Restaura el Base Pointer
	retq                                    # Retorna de la función

LBB0_6:                                     ## Manejo de desbordamiento de búfer
	callq	___stack_chk_fail               # Llama al manejador de fallo del stack
	ud2                                     # Instrucción de aborto
	.cfi_endproc


# ==============================================================================
# FUNCIÓN: thread_routine_two
# ==============================================================================
	.globl	_thread_routine_two
	.p2align	4, 0x90
_thread_routine_two:
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$80, %rsp                       # Reserva 80 bytes en el stack

	# --- Protección del Stack ---
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)

	movq	%rdi, -56(%rbp)                 # Guarda el parámetro de entrada

	# --- Copia de cadena local "soy la mera verga en la programacion \n" ---
	leaq	-48(%rbp), %rdi                 # Destino en stack
	leaq	L___const.thread_routine_two.buf(%rip), %rsi # Origen
	movl	$39, %edx                       # Tamaño de 39 bytes
	callq	_memcpy                         # Copia el texto al buffer local

	movq	$0, -72(%rbp)                   # Inicializa i = 0

LBB1_1:                                     ## Encabezado del bucle (for i < 1000)
	cmpq	$1000, -72(%rbp)
	jae	LBB1_4

## %bb.2:                                   ## Cuerpo del bucle
	# --- SECCIÓN CRÍTICA: Decremento protegido por Mutex ---
	leaq	_mutex(%rip), %rdi
	callq	_pthread_mutex_lock             # Bloquea el mutex

	movl	_global_counter(%rip), %eax     # Lee _global_counter
	addl	$-1, %eax                       # Le resta 1 (DECREMENTO)
	movl	%eax, _global_counter(%rip)     # Escribe el nuevo valor

	leaq	_mutex(%rip), %rdi
	callq	_pthread_mutex_unlock           # Libera el mutex

	# --- Operación I/O: Escritura en archivo ---
	leaq	L_.str(%rip), %rdi              # Ruta del archivo
	movl	$9, %esi                        # Modo de apertura
	movb	$0, %al
	callq	_open                           # Abre el archivo

	movl	%eax, -60(%rbp)                 # Guarda fd
	movl	-60(%rbp), %edi                 # 1er arg: fd
	leaq	-48(%rbp), %rsi                 # 2do arg: buffer local
	movl	$38, %edx                       # 3er arg: 38 bytes
	callq	_write                          # Escribe la cadena
	movl	-60(%rbp), %edi
	callq	_close                          # Cierra el archivo

## %bb.3:                                   ## Incremento del bucle
	movq	-72(%rbp), %rax
	addq	$1, %rax                        # i++
	movq	%rax, -72(%rbp)
	jmp	LBB1_1

LBB1_4:                                     ## Salida de la función y verificación
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rcx
	cmpq	%rcx, %rax
	jne	LBB1_6

## %bb.5:
	xorl	%eax, %eax                      # Retorna NULL
	addq	$80, %rsp
	popq	%rbp
	retq

LBB1_6:
	callq	___stack_chk_fail
	ud2
	.cfi_endproc


# ==============================================================================
# FUNCIÓN PRINCIPAL: main
# ==============================================================================
	.globl	_main
	.p2align	4, 0x90
_main:
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp                       # Reserva 32 bytes en stack
	movl	$0, -4(%rbp)                    # Código de retorno predeterminado = 0
	movl	%edi, -8(%rbp)                  # Guarda argc
	movq	%rsi, -16(%rbp)                 # Guarda argv

	# --- Creación del Hilo 1 ---
	leaq	-24(%rbp), %rdi                 # Puntero a t1 (&t1)
	xorl	%eax, %eax
	movl	%eax, %ecx
	leaq	_thread_routine(%rip), %rdx     # Función objetivo
	movq	%rcx, %rsi                      # Atributos NULL
	callq	_pthread_create
	cmpl	$0, %eax                        # Verifica retorno == 0
	je	LBB2_2

## %bb.1:                                   ## Error al crear hilo 1
	movl	$-1, -4(%rbp)
	jmp	LBB2_5

LBB2_2:                                     ## Creación del Hilo 2
	leaq	-32(%rbp), %rdi                 # Puntero a t2 (&t2)
	xorl	%eax, %eax
	movl	%eax, %ecx
	leaq	_thread_routine_two(%rip), %rdx # Función objetivo
	movq	%rcx, %rsi                      # Atributos NULL
	callq	_pthread_create
	cmpl	$0, %eax                        # Verifica retorno == 0
	je	LBB2_4

## %bb.3:                                   ## Error al crear hilo 2
	movl	$-1, -4(%rbp)
	jmp	LBB2_5

LBB2_4:                                     ## Sincronización e Impresión
	movq	-24(%rbp), %rdi
	xorl	%eax, %eax
	movl	%eax, %esi
	callq	_pthread_join                   # Espera finalización de t1

	movq	-32(%rbp), %rdi
	xorl	%eax, %eax
	movl	%eax, %esi
	callq	_pthread_join                   # Espera finalización de t2

	movl	_global_counter(%rip), %esi     # Carga el resultado final en %esi
	leaq	L_.str.1(%rip), %rdi            # Cadena de formato "Valor final..."
	movb	$0, %al
	callq	_printf                         # Imprime el valor final

	leaq	_mutex(%rip), %rdi
	callq	_pthread_mutex_destroy          # Destruye el mutex
	movl	$0, -4(%rbp)

LBB2_5:                                     ## Epílogo de main
	movl	-4(%rbp), %eax                  # Código de salida (0 o -1)
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc


# ==============================================================================
# SECCIÓN DE DATOS E IMPRESIONES
# ==============================================================================
	.section	__DATA,__data

	.globl	_global_counter                 ## Variable global de contador
	.p2align	2
_global_counter:
	.long	20                              ## Inicializado en 20 (0x14)

	.globl	_mutex                          ## Variable global de Mutex
	.p2align	3
_mutex:
	.quad	850045863                       ## Valor interno de inicialización
	.space	56                              ## Reserva de espacio para la estructura pthread_mutex_t

# --- Constantes Literales de Cadena ---
	.section	__TEXT,__cstring,cstring_literals
L___const.thread_routine.buf:
	.asciz	"New line \n"

L_.str:
	.asciz	"/Users/vdj/Desktop/Cyp_2026/README.txt"

	.p2align	4
L___const.thread_routine_two.buf:
	.asciz	"soy la mera verga en la programacion \n"

L_.str.1:
	.asciz	"Valor final de global_counter: %d\n"

.subsections_via_symbols