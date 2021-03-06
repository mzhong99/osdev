; ------------------------------------------------------------------------------
; Define constants for the mutliboot header
; ------------------------------------------------------------------------------
MBALIGN  equ 1 << 0              ; Align loaded modules on page boundaries
MEMINFO  equ 1 << 1              ; Provides memory map
FLAGS    equ MBALIGN | MEMINFO   ; This is the multiboot 'flag' field
MAGIC    equ 0x1BADB002          ; 'magic number' lets bootloader find header
CHECKSUM equ -(MAGIC + FLAGS)    ; Checksum of above to prove we're multiboot

; ------------------------------------------------------------------------------
; Declares a multiboot header that marks this program as a kernel. These are
; magic numbers that are documented in the multiboot standard. The bootloader
; will search for this signature in the first 8 KiB of the kernel file, aligned
; at a 32-bit boundary. The signature is in its own section so that the header
; can be forced to be within the first 8 KiB of the kernel file. 
; (1KiB = 1024 bytes)
; ------------------------------------------------------------------------------

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; ------------------------------------------------------------------------------
; Since the multiboot standard does not define the value of the stack pointer
; register (esp), we need to define space for a stack to exist. The following
; lines allocate room for a small stack by:
; 
; 1.) Creating a symbol at the bottom of the stack
; 2.) Allocating 16384 bytes for it (16 KiB)
; 3.) Creating a sumbol at the top
; 
; In x86, stacks grow DOWNWARDs. This stack is in its own section so that it can
; be marked nobits, which means the kerned file itself is smaller since it does
; not contain the stack. The stack in x86 must be 16-byte aligned. 
; ------------------------------------------------------------------------------

section .bss
align 16

stack_bottom:
resb 16384
stack_top:

; ------------------------------------------------------------------------------
; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is no longer
; relevant.
; ------------------------------------------------------------------------------

section .text
global _start:function (_start.end - _start)
_start:

    ; --------------------------------------------------------------------------
    ; Initialize the stack by setting the stack pointer to the top of the stack
    ; as defined above
    mov esp, stack_top

    ; --------------------------------------------------------------------------
    ; This is a good place to initialize crucial processor state, like features
    ; such as the GDT, paging, FPU, etc

    
    ; --------------------------------------------------------------------------
    ; Enter the high-level kernel.
    extern kernel_main
    call kernel_main

    ; --------------------------------------------------------------------------
    ; If the system has nothing more to do, put the computer into an infinite
    ; loop by disabling interrupts, waiting for any unmaskable interrupts, and
    ; jumping to re-receive more interrupts
    cli
.hang:
    hlt
    jmp .hang

.end:
