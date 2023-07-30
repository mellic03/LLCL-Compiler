%include "linux64.inc"

section .data
	
section .bss
	
section .text
	global _start


LLCL_FN_main:
	;---------------------; Function prologue
	push rbp
	mov rbp, rsp
	sub rsp, 	48
	;---------------------------------------;

    mov qword [rbp + 32], 2
    mov qword [rbp + 40], 2
    mov qword [rbp + 48], 7

    ; push two 8-byte function parameters,
    ; param 1 is stored at rbp + 24
    ; param 2 is stored at rbp + 16
    ;
    ; BUT,
    ; local var 1 is stored at rbp + 32
    ; local var 2 is stored at rbp + 40
    ; local var 3 is stored at rbp + 48

    mov rax, qword [rbp + 24]
    sub rax, qword [rbp + 40]
    push rax

	pop rax ; Return value



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

