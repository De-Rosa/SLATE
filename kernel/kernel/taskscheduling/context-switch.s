global switch_tasks
extern current_task_TCB
extern TSS

section .text

switch_tasks:
    ;save the current contents of the registers for the current process
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15

    ;change pointer to the next task's stack
    mov rdx, [rel current_task_TCB]
    mov [rdx], rsp
    mov [rel current_task_TCB], rdi
    mov rsp, [rdi]

    ;load new cr3 (only if needed)
    mov rax, [rdi + 8]
    mov rcx, cr3
    cmp rax, rcx
    je .skip_cr3
    mov cr3, rax
.skip_cr3:

    ;update TSS.rsp0 (pointer to the current task’s kernel stack top)
    mov rbx, [rdi + 16]
    mov [rel TSS + 4], rbx

    ;restore the current contents of the registers for the current process
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx

    ret