/*
float fooA (float a, int b) {
  return sin(a) + b;
}

Dicionário
float a		xmm0
int b		edi
*/

.text
.globl fooA

fooA:
	pushq   %rbp
	movq    %rsp, %rbp

	cvtss2sd	%xmm0, %xmm0
	cvtsi2sd	%edi, %xmm2

	call	sin

	addsd	%xmm2, %xmm0

	cvtsd2ss	%xmm0, %xmm0

	leave ret

ou

.text
.glob fooA

fooA:
	pushq   %rbp
	movq    %rsp, %rbp
	subq	$16, %rsp

	cvtss2sd	%xmm0, %xmm0
	cvtsi2sd	%edi, %xmm1

	movsd    %xmm1, -8(%rbp)
	call	sin

	addsd    -8(%rbp), %xmm0

	cvtsd2ss	%xmm0, %xmm0

	leave
	ret
	