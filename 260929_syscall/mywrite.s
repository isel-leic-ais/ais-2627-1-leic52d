	.text
	.global	mywrite
mywrite:
	mov	%rdi, %rdi
	mov	%rsi, %rsi
	mov	%rdx, %rdx
	mov	$1, %rax
	syscall
	ret

	.section .note.GNU-stack 
	
