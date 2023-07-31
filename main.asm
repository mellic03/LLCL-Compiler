%include "linux64.inc"

section .data
	
section .bss
	
section .text
	global _start
	
LLCL_FN_main:
	;---------------------; Function prologue
	push qword rbp
	mov rbp, rsp
	sub rsp, 	48	
	;---------------------------------------;
	;---------------------; Function body
	push qword 15
	pop qword rax
	mov qword [rbp + 16], rax
	push qword 5
	pop qword rax
	mov qword [rbp + 24], rax
	push qword 1
	pop qword rax
	mov qword [rbp + 32], rax
	
	mov rcx, 0
	mov rsi, 3
	cmp rcx, rsi
	jge .LLCL_LOOP_END_0
	.LLCL_LOOP_BEGIN_0:
		push qword [rbp + 32]
		push qword 2
		pop rdx
		pop rax
		mul rdx
		push rax
		pop qword rax
		mov qword [rbp + 32], rax
		add rcx, 1  ; increment loop counter
		cmp rcx, rsi
		jge .LLCL_LOOP_END_0
		jmp .LLCL_LOOP_BEGIN_0
	.LLCL_LOOP_END_0:
	
	mov rcx, 0
	mov rsi, 3
	cmp rcx, rsi
	jge .LLCL_LOOP_END_1
	.LLCL_LOOP_BEGIN_1:
		push qword [rbp + 32]
		push qword 1
		pop rdx
		pop rax
		add rax, rdx
		push rax
		pop qword rax
		mov qword [rbp + 32], rax
		add rcx, 1  ; increment loop counter
		cmp rcx, rsi
		jge .LLCL_LOOP_END_1
		jmp .LLCL_LOOP_BEGIN_1
	.LLCL_LOOP_END_1:
	push qword [rbp + 32]
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

