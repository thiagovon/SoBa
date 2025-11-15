/*
double fooB (int a, float b) {
  return (a+b)*(a-b);
} 
Dicionário
int a 	edi - xmm1 e xmm2
float b 	xmm0
*/

.globl
.text fooB

fooB:
	pushq	%rbp
	movq	%rsp, %rbp

	cvtsi2ss	%edi, %xmm1

	movss 	%xmm1, %xmm2 #preservar o valor para as duas operações

	addss	%xmm0, %xmm1 #(a+b)
	subss	%xmm0, %xmm2 #(a-b)

	mulss	%xmm2, %xmm1 #(a+b)*(a-b)

	cvtss2sd		%xmm1, %xmm1 #converter pra double

	leave
	ret