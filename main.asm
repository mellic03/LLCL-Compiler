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
	push qword 0
	pop qword rax
	mov qword [rbp + 16], rax
	push qword 0
	pop qword rax
	mov qword [rbp + 24], rax
	
	push  r9   ; loop counter
	push  rcx  ; end value
	mov   r9,  0
	mov   rcx, 5
	cmp   r9,   rcx
	jge   .LLCL_LOOP_END_1_0
	.LLCL_LOOP_BEGIN_1_0:
		
		push  r9   ; loop counter
		push  rcx  ; end value
		mov   r9,  0
		mov   rcx, 5
		cmp   r9,   rcx
		jge   .LLCL_LOOP_END_1_1
		.LLCL_LOOP_BEGIN_1_1:
			push qword [rbp + 24]
			push qword 1
			pop r8
			pop rax
			add rax, r8
			push rax
			pop qword rax
			mov qword [rbp + 24], rax
			add   r9,  1  ; increment loop counter
			cmp   r9,  rcx
			jge   .LLCL_LOOP_END_1_1
			jmp   .LLCL_LOOP_BEGIN_1_1
		.LLCL_LOOP_END_1_1:
			pop   rcx
			pop   r9
		push qword [rbp + 16]
		push qword 1
		pop r8
		pop rax
		add rax, r8
		push rax
		pop qword rax
		mov qword [rbp + 16], rax
		add   r9,  1  ; increment loop counter
		cmp   r9,  rcx
		jge   .LLCL_LOOP_END_1_0
		jmp   .LLCL_LOOP_BEGIN_1_0
	.LLCL_LOOP_END_1_0:
		pop   rcx
		pop   r9
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

