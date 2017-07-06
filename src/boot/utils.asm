global reload_segment
global switch_context
global call_ring3

section .text
reload_segment:
    sub rsp, 16
    mov qword [rsp + 8], 8
    mov qword [rsp], target
    db 0x48
    retf

    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
target:
    ret


switch_context:
    push rbp
    push rbx
    push r12
    push r13
    push r14
    push r15
    push rdi
    push rsi

    mov [rdi], rsp
    mov rsp, [rsi]

    pop rsi
    pop rdi
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp

    ret

extern set_up_ring0_rsp

; rdi = func  rsi = rsp
call_ring3:
    mov ax, 24 | 3
    mov ds, ax
    mov es, ax
    and rsp, 0xFFFFFFFFFFFFFFF0
    push qword 24 | 3
    push rsi
    push qword 1 << 9
    push qword 32 | 3
    push rdi
    mov rdi, rsp
    call set_up_ring0_rsp
    iretq

extern syscall_entry
extern save_user_get_kernel_rsp
extern get_user_rsp
global syscall_stub

syscall_stub:
    cli
    push rcx
    push r11
    ;
    push rax
    push rdi
    push rsi
    push rdx
    push r10
    push r8
    push r9
    push r15
    mov r15, rsp
    add r15, 8
    push r14
    push r13
    push r12
    push rbp
    push rbx
    ;
    mov rdi, rsp
    call save_user_get_kernel_rsp
    mov rsp, rax
    ;
    mov rdi, r15
    sti
    call syscall_entry
    cli
    ;
    call get_user_rsp
    mov rsp, rax
    pop rbx
    pop rbp
    pop r12
    pop r13
    pop r14
    pop r15
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    ;
    pop r11
    pop rcx
    o64 sysret


global user_exit_stub
global end_user_exit_stub
user_exit_stub:
    mov rax, 60
    mov rsi, 0
    syscall
end_user_exit_stub: