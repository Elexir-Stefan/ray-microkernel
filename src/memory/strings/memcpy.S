#
# memcpy.S
#

	.text
	.globl	memcpy
	.type	memcpy, @function
	.func
memcpy:
	push %ebp
	mov %esp, %ebp
	pushl	%esi
	pushl	%edi
#ifdef REGPARM
	movl	%edx, %esi
#else
	movl	8(%ebp), %eax
	movl	12(%ebp), %esi
	movl	16(%ebp), %ecx
#endif
	movl	%eax, %edi
	movl	%ecx, %edx

	shrl	$2, %ecx
	cld
	rep ; movsl

	jnc	1f		# The shrl had carry out if odd word count
	movsw
1:
	testb	$1, %dl
	jz	2f
	movsb
2:
	popl	%edi
	popl	%esi
	pop %ebp
	ret
.endfunc
