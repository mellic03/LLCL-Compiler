%include "linux64.inc"

section .data
	
section .bss
	
section .text
	global _start


LLCL_FN_main:
	;---------------------; Function prologue
	push rbp
	mov rbp, rsp
	sub rsp, 	16
	;---------------------------------------;

    ; for 15 .. 25 ;------------------------;

    mov r8, 0

    mov rcx, 15
    mov rdx, 25

    cmp rcx, rdx
    jge .LLCL_LOOP_END_0

    .LLCL_LOOP_BEGIN_0:

        add r8, 1

        add rcx, 1
        cmp rcx, rdx
        jge .LLCL_LOOP_END_0    ; end if rcx >= rdx
        jmp .LLCL_LOOP_BEGIN_0  ; else, continue loop

    .LLCL_LOOP_END_0:

    ;---------------------------------------;

    mov rax, r8

	;---------------------; Function epilogue
	mov rsp, rbp
	pop rbp
	ret
	;---------------------------------------;



_start:
    push 10
    push 5
	call LLCL_FN_main
	exit rax

