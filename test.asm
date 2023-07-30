%include "linux64.inc"

section .data
	
section .bss
	
section .text
	global _start
	
LLCL_FN_alloc:
	;---------------------; Function prologue
	push qword rbp
	mov rbp, rsp
	sub rsp, 	16	
	;---------------------------------------;
	;---------------------; Function body
	;-----------------------------------;
	;---------------------; Function epilogue
	mov rsp, rbp
	pop rbp
	ret
	;---------------------------------------;
LLCL_FN_main:
	;---------------------; Function prologue
	push qword rbp
	mov rbp, rsp
	sub rsp, 	64	
	;---------------------------------------;
	;---------------------; Function body
	push qword 1
	pop qword rax
	mov qword [rbp + 16], rax
	push qword 2
	pop qword rax
	mov qword [rbp + 24], rax
	push qword 3
	pop qword rax
	mov qword [rbp + 32], rax
	push qword 4
	pop qword rax
	mov qword [rbp + 40], rax
	push qword [rbp + 16]
	push qword [rbp + 24]
	pop rdx
	pop rax
	add rax, rdx
	push rax
	pop qword rax
	mov qword [rbp + 48], rax
	push qword 8
	push qword 10
	push qword 15
	pop rdx
	pop rax
	sub rax, rdx
	push rax
	pop rdx
	pop rax
	mul rdx
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

