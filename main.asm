%include "linux64.inc"

section .data
	
section .bss
	
section .text
	global _start
	
LLCL_FN_main:
	;---------------------; Function prologue
	push qword rbp
	mov rbp, rsp
	sub rsp, 	40	
	;---------------------------------------;
	;---------------------; Function body
	push qword 10
	pop qword rax
	mov qword [rbp + 16], rax
	push qword 2
	pop qword rax
	mov qword [rbp + 24], rax
	
	mov rcx, 0
	mov rsi, 5
	cmp rcx, rsi
	jge .LLCL_LOOP_END_0
	.LLCL_LOOP_BEGIN_0:
		push qword [rbp + 16]
		push qword 1
		pop r8
		pop rax
		sub rax, r8
		push rax
		pop qword rax
		mov qword [rbp + 16], rax
		push qword [rbp + 24]
		push qword 1
		pop r8
		pop rax
		add rax, r8
		push rax
		pop qword rax
		mov qword [rbp + 24], rax
		add rcx, 1  ; increment loop counter
		cmp rcx, rsi
		jge .LLCL_LOOP_END_0
		jmp .LLCL_LOOP_BEGIN_0
	.LLCL_LOOP_END_0:
	push qword [rbp + 16]
	push qword [rbp + 24]
	pop r8
	pop rax
	add rax, r8
	push rax
	pop rax
	;-----------------------------------;
	;---------------------; Function epilogue
	mov rsp, rbp
	pop rbp
	ret
	;---------------------------------------;
_start:
	push 1
	push 0
	call LLCL_FN_main
	exit rax

