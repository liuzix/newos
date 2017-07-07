
extern do_irq

section .text


%macro save_all 0
    push rbp
    mov rbp, rsp
    push rax
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
%endmacro

%macro restore_all 0
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop rax
    pop rbp
%endmacro

%macro intr_handler_gen 1
section .text
global intr_handler_%1

intr_handler_%1:
    save_all
    mov rdi, %1
    call do_irq
    restore_all
    iretq
%endmacro


%assign i 0
%rep    256
  intr_handler_gen i
%assign i i+1
%endrep


global page_fault_stub
extern page_interrupt_handler

page_fault_stub:
    save_all
    sub rsp, 8
    call page_interrupt_handler
    add rsp, 8
    restore_all
    add rsp, 8
    iretq

global GP_fault_stub
extern GP_interrupt_handler

GP_fault_stub:
    add rsp, 8
    save_all
    call GP_interrupt_handler
    restore_all
    iretq

global double_fault_stub
extern double_interrupt_handler

double_fault_stub:
    save_all
    call double_interrupt_handler
    restore_all
    iretq